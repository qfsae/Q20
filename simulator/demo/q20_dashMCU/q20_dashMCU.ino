/* Code going from dash MCU CAN to dash Teensy

- Receive gear, tps and rpm over CAN
- Send ^ over Serial to Teensy
*/

#include "mcp_can.h"
#include <SPI.h>

#define SPI_CS_PIN 10

MCP_CAN CAN(SPI_CS_PIN);

unsigned long rpmID = 0x00;
unsigned long tpsID = 0x08;
unsigned long gearID = 0xC0;
byte gear;
int rpm;
float tps;

unsigned char message[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
 Serial.begin(115200);
 Serial.println("Starting");
 while (CAN.begin(CAN_250KBPS) != CAN_OK) {
   Serial.println("CAN BUS init failure");
   Serial.println("Trying again");
   delay(100);
 }
 Serial.println("CAN Bus Initialized!");
}

void loop() {
  if (!SENDING) {
    /* This stuff needs to get updated to recieve msgs from the ETC as well, and transmit all of them over CAN with unique ID's */
    unsigned char len = 0;
    unsigned char buf[8];
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&len, buf);
      unsigned long id = CAN.getCanId();
      Serial.write(buf[0]);
    }  
  } else {
      unsigned long sendingID = 0x00;
      CAN.sendMsgBuf(sendingID, 0, 8, message);
      delay(100);
      message[0]++;
  }
}
