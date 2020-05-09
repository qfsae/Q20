#include "mcp_can.h"
#define SPI_CS_PIN 9

// receive addr for CAN messages
#define ADDR 0x00

MCP_CAN CAN(SPI_CS_PIN);
unsigned char len = 8;
unsigned char message[8];

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(10000);
  }
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, message);
    unsigned long id = CAN.getCanId();
    if (id == 0) {
      Serial.print("gear = ");
      Serial.println(message[0]);
    }
  }
}
