#include <EEPROM.h>
#include <SPI.h>
#include <GD23Z.h>

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

char buf[2];
int rpm;
int gear;
int lastGear = 0;
float tps;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  GD.begin(0);
  startMillis = millis();  //initial start time
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    unsigned char bytecount = 0;
    while (Serial.available() && bytecount < 10) {
        rc = Serial.read();
//        if (recvInProgress == true) {
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
//        }
//
//        else if (rc == startMarker) {
//            recvInProgress = true;
//        }

        bytecount++;
    }
    receivedChars[ndx] = '\0';
    ndx = 0;
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

void loop() {
  lastGear = gear;
  recvWithStartEndMarkers();
//    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        //showParsedData();
        newData = false;
//    }
    
//  ltoa(currentMillis-startMillis,buf,10);
//  startMillis = currentMillis;
//  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
//  
  
  GD.ClearColorRGB(0,0,0);
  GD.Clear();
  GD.cmd_gauge(150, 136, 100, OPT_NOPOINTER, 8, 5, 0, 7500);
  GD.ColorRGB(255,0,0);
  GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, rpm, 7500);
  GD.ColorRGB(255,255,255);
//  GD.cmd_text(25, 25, 25, 0, receivedChars);
  GD.cmd_text(130, 240, 24, 0, "RPM");
  GD.cmd_number(150, 180, 24, OPT_CENTER, rpm);
  GD.cmd_number(350, 136, 31, OPT_CENTER, gear);
  GD.swap();
}
