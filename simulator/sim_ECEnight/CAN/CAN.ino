/* This file will run on the Arduino that is sending CAN messages from the sim to our prototypes */

#include "mcp_can.h"
#include <SPI.h>

#define SPI_CS_PIN 9 // double check if 9 or 10 for specific shield

MCP_CAN CAN(SPI_CS_PIN);

byte gear;
int rpm;
float tps;
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];            // temporary array for use when parsing

boolean newData = false;

void setup() {
  Serial.begin(115200);
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    delay(100);
  }
//   Serial.println("CAN Bus Initialized!");
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
      gear = temp2 - 1;
    } else { gear = -1; }

    strtokIndx = strtok(NULL, ",");
    float tempf = atof(strtokIndx);     // convert this part to a int
    if(tempf != 0.0) {
      tps = tempf;
    }
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

    if(CAN_MSGAVAIL == CAN.checkReceive()){
      
    } else {
        unsigned char message[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned long sendingID = 0x00;
        message[0] = gear;
        message[1] = tps;
        message[2] = rpm;

        CAN.sendMsgBuf(sendingID, 0, 8, message);
    }
}
