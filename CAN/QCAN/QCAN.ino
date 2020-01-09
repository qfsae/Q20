/*
  ECU Testbench
  This sketch collects all the Data the ECU broadcasts and prints it to serial
  using the library. There is method for performing a formatted print of each
  address listed on the ECU Datasheet and its message contents. This sketch must
  be used in tandem with a running PE3 ECU for testing the running vehicle.

  NOTE: Currently only the PE1 - PE7 addresses are supported by the ECU library
*/

#include "DEFS.h"
#include "ECU.h"
#include "mcp_can.h"

// Use pin 10 for CAN shield version 1.2
#define SPI_CS_PIN 10

MCP_CAN CAN(SPI_CS_PIN);
ECU ECU;

unsigned long id;
unsigned char buf[8];
unsigned char len;

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(10000);
  }
  Serial.println("Initialization Success");
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    id = CAN.getCanId();
    ECU.update(id, buf, len);
    ECU.debugPrint(id);
  }
}
