#include "mcp_can.h"

// Use pin 10 for CAN shield version 1.2
#define SPI_CS_PIN 10
// NOTE: Base Address for MOTEC M150 CAN is 0x07F0

MCP_CAN CAN(SPI_CS_PIN);

unsigned long id;
unsigned char buf[8];
unsigned char len;

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(1000);
  }
  Serial.println("Initialization Success");
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    id = CAN.getCanId();
    Serial.print("Getting Data from ID: 0x");
    Serial.println(id, HEX);
    for (int i = 0; i < len; i++) {
      Serial.print(buf[i]);
      Serial.print("\t");
    }
    Serial.println();
  }
}
