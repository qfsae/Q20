#include "mcp_can.h"
#include <SPI.h>

#define SENDING 0

#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);
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
