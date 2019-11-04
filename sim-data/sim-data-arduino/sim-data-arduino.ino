#include <SPI.h>
#include <MCUFRIEND_kbv.h>

#define LCD_CS     A3                                                // Set CS Pin for LCD
#define LCD_CD     A2                                                // Set Command/Data pin for LCD
#define LCD_WR     A1                                                // Set Write pin for LCD
#define LCD_RD     A0                                                // Set Read pin for LCD
#define LCD_RESET  A4                                                // Set Reset pin for LCD

MCUFRIEND_kbv tft;                                                   // Init TFT

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//declare status variables
int rpm;
float tps;
float batVolt = 13.22;
int gear;
int engTemp = 25;


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];            // temporary array for use when parsing

boolean newData = false;

void setup() {
  Serial.begin(115200);

  /*
   * Set up LCD
   */
  tft.reset();
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;     // write only shield
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.fillRect(0,0,320,30, WHITE);   // top toolbar
  
  tft.setCursor(50,40);
  tft.setTextSize(2);
  tft.print("GEAR:");

  tft.setCursor(175, 40);
  tft.print("ENG. TEMP:");

  tft.setCursor(175, 110);
  tft.print("BAT. VOLT:");

  tft.setCursor(175, 180);
  tft.print("OIL PRES:");

}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    int temp = atoi(strtokIndx); // copy it to messageFromPC
    if(temp != 0) {
      rpm = temp;
    }
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    int temp2 = atoi(strtokIndx); // convert this part to an float
    if(temp2 != 0) {
      gear = temp2;               
    }

    strtokIndx = strtok(NULL, ",");
    float tempf = atof(strtokIndx);     // convert this part to a int
    if(tempf != 0.0) {
      tps = tempf;
    }

}

int lastMillis = 0;

void testDataValues() {  
  if(millis() - lastMillis > 500 && engTemp < 130) {
    lastMillis = millis();
    engTemp += 1;
  }
  
  if(engTemp < 60) {
    tft.setTextColor(WHITE, BLUE);
  } else if(engTemp > 60 && engTemp < 100) {
    tft.setTextColor(WHITE, YELLOW);
  } else if(engTemp > 100 && engTemp < 115) {
    tft.setTextColor(WHITE, GREEN);
  } else if(engTemp > 115) {
    tft.setTextColor(WHITE, RED);
  }

  tft.setCursor(185, 60);
  tft.setTextSize(5);
  char buf[4];
  sprintf(buf, "%03d", engTemp);
  tft.print(buf);

  tft.setCursor(170, 130);
  tft.setTextColor(WHITE, GREEN);
  char batBuf[4];
  int batVoltInt = batVolt*10;
  sprintf(batBuf, "%02d.%1d", (batVoltInt/10), (batVoltInt%10));
  tft.print(batBuf);

  tft.setCursor(185, 200);
  tft.setTextColor(WHITE, RED);
  tft.print("yes");
}


void updateTFT() {
  tft.setCursor(50,50);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(6);
//  char buf[4];
//  sprintf(buf, "%04d", rpm);
////  tft.fillRect(50, 50, 200, 50, BLACK);
//  tft.print(buf);

//  tft.setCursor(50,100);
////  tft.fillRect(50, 100, 200, 50, BLACK);
//  tft.print(tps);

//==================== PRINT GEAR 
  tft.setCursor(25,75);
  tft.setTextSize(20);
//  tft.fillRect(50, 150, 200, 50, BLACK);
  if(gear > 1) {
    tft.print(gear-1);
  } 
//  delay(5);

  testDataValues();
}

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        //showParsedData();
        newData = false;
    }
    updateTFT();
}
