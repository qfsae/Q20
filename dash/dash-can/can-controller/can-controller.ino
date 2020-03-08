#include "mcp_can.h"
#include <SPI.h>

#define BODY_LENGTH 8

#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);

unsigned char msg[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int endMarker = -20000;
char *c = (char *)&endMarker;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");

}

void writeMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.write(msg[i]);
  }
  Serial.write(c[0]);
  Serial.write(c[1]);
}

void loop() {
    unsigned char len = 0;
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&len, msg);
      unsigned long id = CAN.getCanId();
     if (id == 0x7F0) {
     	// msg[2] is the battery voltage divided by 0.1216
	writeMsg();
     }
   }
}
