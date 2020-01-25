#include <SPI.h>
#include <Wire.h>

int gear = 0;
int rpm = 0;
float tps = 0.0;

// Shift Registers 
#define latchPIN    4 // pin 3 on ATMega328P
#define clockPIN    7 // pin 11 on ATMega328P
#define dataPIN     3 // pin 1 on ATMega328P
#define pinQ0       1
#define pinQ1       2
#define pinQ2       4
#define pinQ3       8
#define pinQ4      16
#define pinQ5      32
#define pinQ6      64
#define pinQ7     128
int SR1 = 0, SR2 = 0, SR3 = 255, SR4 = 255, SR5 = 255, SR6 = 255;

// Shift Lights
#define sRPM1    6800
#define sRPM2    7400
#define sRPM3    8000
#define sRPM4    8600
#define sRPM5    9200
#define sRPM6    9800
#define sRPM7   10400
#define sRPM8   11000
#define sRPM9   11600
#define sRPM10  12000
#define sRPM11  12200
#define sRPM12  12300
#define sLED1   pinQ2 //SR2 
#define sLED2   pinQ3 //SR2 
#define sLED3   pinQ4 //SR2 
#define sLED4   pinQ5 //SR2 
#define sLED5   pinQ6 //SR2
#define sLED6   pinQ7 //SR2
#define sLED7   pinQ7 //SR1
#define sLED8   pinQ6 //SR1
#define sLED9   pinQ2 //SR1
#define sLED10  pinQ3 //SR1
#define sLED11  pinQ4 //SR1
#define sLED12  pinQ5 //SR1
#define rpmFlashTime 75 // RPM Flash timing (ms)

// Warning Lights
#define ledLR  pinQ4
#define ledLG  pinQ3
#define ledLB  pinQ2
#define ledRR  pinQ7
#define ledRG  pinQ6
#define ledRB  pinQ5
#define engTempLow        60    //Blue Blink
#define engTempMedLow     70    //Blue
#define engTempMedHigh    95    //Yellow
#define engTempHigh       105   //Red
#define engTempFlashTime  500   //Time in ms
#define batVoltMed        13    //Yellow
#define batVoltLow        12.5  //Red
int launchArm = 0;

// 7 segment
#define ssC pinQ1 //SR3
#define ssD pinQ2 //SR3  
#define ssE pinQ3 //SR3
#define ssG pinQ4 //SR3
#define ssF pinQ5 //SR3
#define ssA pinQ6 //SR3
#define ssB pinQ7 //SR3
int ssCount = 1;
int seg1 = (ssB + ssC); // 1
int seg2 = (ssA + ssB + ssD + ssE + ssG); // 2
int seg3 = (ssA + ssB + ssC + ssD + ssG); // 3
int seg4 = (ssB + ssC + ssF + ssG); // 4
int seg5 = (ssA + ssC + ssD + ssF + ssG); // 5
int seg6 = (ssA + ssC + ssD + ssE + ssF + ssG); // 6

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];    
boolean newData = false; 

// Inputs 
#define cyclePin     A0
int pitCom = 0;
bool startUp = false;

// Top level initialization
void setup() {
  Serial.begin(115200);
  Serial.println("--------- Upper Dash V2 ---------");
  initShiftRegisters(); // Initialize Shift Registers
//  initSequence1(); // Light show
//  initSequence2();
  delay(100);
  pinMode(13, OUTPUT);
}

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

//    strtokIndx = strtok(NULL, ",");
//    int tempmr = atof(strtokIndx);
//    if(tempmr != 0) {
//      rpm = tempmr;
//    }
}

// Main sequence
void loop() {
  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        newData = false;
    }

  if(rpm >= 5000) {
    digitalWrite(13, HIGH);
  }
  resetSR(); // Reset shift register values
  shiftLights(); // update SR1, SR2
  updateDash();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
//        Serial.println(rc);

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

// Initialize Shift Registers
void initShiftRegisters(){
  pinMode(latchPIN, OUTPUT);
  pinMode(clockPIN, OUTPUT);
  pinMode(dataPIN, OUTPUT);
  pinMode(cyclePin, INPUT);
  digitalWrite(latchPIN, LOW);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
  digitalWrite(latchPIN, HIGH);
}

/* more cool stuff */
void initSequence1() {
  // light each shift light one at a time up and down
  for (int i = 1; i <= 24; i++) {
    switch (i) {
      case 1: SR2 += sLED1; break;
      case 2: SR2 += sLED2; break;
      case 3: SR2 += sLED3; break;
      case 4: SR2 += sLED4; break;
      case 5: SR2 += sLED5; break;
      case 6: SR2 += sLED6; break;
      case 7: SR1 += sLED7; break;
      case 8: SR1 += sLED8; break;
      case 9: SR1 += sLED9; break;
      case 10: SR1 += sLED10; break;
      case 11: SR1 += sLED11; break;
      case 12: SR1 += sLED12; break;
      case 13: SR1 -= sLED12; break;
      case 14: SR1 -= sLED11; break;
      case 15: SR1 -= sLED10; break;
      case 16: SR1 -= sLED9; break;
      case 17: SR1 -= sLED8; break;
      case 18: SR1 -= sLED7; break;
      case 19: SR2 -= sLED6; break;
      case 20: SR2 -= sLED5; break;
      case 21: SR2 -= sLED4; break;
      case 22: SR2 -= sLED3; break;
      case 23: SR2 -= sLED2; break;
      case 24: SR2 -= sLED1; break;
    }
    //randomize colors on the Status LEDS
    int colours[] = {111, 183, 219}; // red green blue
    randomSeed(analogRead(0));
    SR4 = colours[random(0, 3)];
    SR5 = colours[random(0, 3)];
    SR6 = colours[random(0, 3)];

    digitalWrite(latchPIN, LOW);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
    digitalWrite(latchPIN, HIGH);

    delay(25);
  }
}

// After CAN init
void initSequence2(){
  for(int j = 0; j < 3; j++){
    for (int i = 1; i <= 15; i++) {
      switch (i) {
        case 1: SR2 += sLED1; break;
        case 2: SR2 += sLED2; break;
        case 3: SR2 += sLED3; break;
        case 4: SR2 += sLED4; SR2 -= sLED1; break;
        case 5: SR2 += sLED5; SR2 -= sLED2; break;
        case 6: SR2 += sLED6; SR2 -= sLED3; break;
        case 7: SR1 += sLED7; SR2 -= sLED4; break;
        case 8: SR1 += sLED8; SR2 -= sLED5; break;
        case 9: SR1 += sLED9; SR2 -= sLED6; break;
        case 10: SR1 += sLED10; SR2 -= sLED7; break;
        case 11: SR1 += sLED11; SR2 -= sLED8; break;
        case 12: SR1 += sLED12; SR2 -= sLED9; break;
        case 13: SR1 -= sLED10; break;
        case 14: SR1 -= sLED11; break;
        case 15: SR1 -= sLED12; break;
      }
      //randomize colors on the Status LEDS
      int colours[] = {111, 183, 219}; // red green blue
      randomSeed(analogRead(0));
      SR4 = colours[random(0, 3)];
      SR5 = colours[random(0, 3)];
      SR6 = colours[random(0, 3)];

      digitalWrite(latchPIN, LOW);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
      digitalWrite(latchPIN, HIGH);
      delay(150);
    }
  }
}

// Reset shift register values
void resetSR(){
  SR1 = 0;
  SR2 = 0;
  SR3 = 255;
  SR4 = 255;
  SR5 = 255;
  SR6 = 255;
}

// Update shift lights
void shiftLights() {
  // Should we add blinking when 12 lights are on??
  if (rpm > sRPM12) {
    // 12 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
  }
  else if (rpm > sRPM11) {
    // 11 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8 + sLED9 + sLED10 + sLED11;
  }
  else if (rpm > sRPM10) {
    // 10 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8 + sLED9 + sLED10;
  }
  else if (rpm > sRPM9) {
    // 9 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8 + sLED9;
  }
  else if (rpm > sRPM8) {
    // 8 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8;
  }
  else if (rpm > sRPM7) {
    // 7 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7;
  }
  else if (rpm > sRPM6) {
    // 6 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
  }
  else if (rpm > sRPM5) {
    // 5 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5;
  }
  else if (rpm > sRPM4) {
    // 4 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4;
  }
  else if (rpm > sRPM3) {
    // 3 lights
    SR2 += sLED1 + sLED2 + sLED3;
  }
  else if (rpm > sRPM2) {
    // 2 lights
    SR2 += sLED1 + sLED2;
  }
  else if (rpm > sRPM1) {
    // 1 lights
    SR2 += sLED1;
  }

//if(rpm > 0) {
//  SR2 += sLED1;
//}
}

// Push changes to dash
void updateDash(){
  digitalWrite(latchPIN, LOW);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
  digitalWrite(latchPIN, HIGH);
}
