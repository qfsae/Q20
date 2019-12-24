/*
  Testing Sketch for the QCAN Library
  Library will build atop the CAN shield library to retrieve ECU and PDM
  messages This sketch will use the underlying library to make a CAN sniffer
  which will print the ECU and PDM messages
*/

#include "DEFS.h"
#include "ECU.h"
#define SENDING 0
#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);
ECU ECU;

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(10000);
  }
  Serial.println("Initialization Success");
  if (SENDING) {
    Serial.print("SENDING ADDR = ");
    Serial.println(PE1, HEX);
  }
  ECU.begin(CAN);
}

void loop() {
  if (SENDING) {
    // SEND FAKE MESSAGES HERE
    // Send 200 rpm and 50 TPS;
    int fakeRPM = 200;
    int fakeTPS = 50;
    unsigned char testMessage[8];
    testMessage[0] = fakeRPM;
    testMessage[2] = fakeTPS;
    CAN.sendMsgBuf(PE1, 1, 8, testMessage);
  } else {
    ECU.update();
    Serial.print("RPM = ");
    Serial.print(ECU.rpm);
    Serial.print(", TPS = ");
    Serial.println(ECU.tps);
  }
}
