#include "DEFS.h"
#include "ECU.h"
#include "mcp_can.h"
#include <SPI.h>

#define SENDING 0

#define BODY_LENGTH 4

#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);
ECU ECU;

unsigned char msg[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  while (CAN.begin(CAN_250KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");

  // TESTING
  int endMarker = -20000;
  char *c = (char *)&endMarker;

  msg[4] = c[0];
  msg[5] = c[1];

  msg[0] = 200;
  msg[1] = 200;
}

void writeMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.write(msg[i]);
  }
  Serial.write(msg[4]);
  Serial.write(msg[5]);
}

void loop() {
  if (!SENDING) {
    unsigned char len = 0;
    unsigned char buf[8];
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&len, buf);
      unsigned long id = CAN.getCanId();
      ECU.update(id, buf, len);
      // ECU.debugPrint(id);
      // abstract this away in an ECU lib func which populates the buffer
      /* msg[0] = (unsigned char) ((int)ECU.tps & 0x00FF); */
      //      msg[1] = (unsigned char) (((int)ECU.tps & 0xFF00) >> 8);
      /* msg[1] = ((int)ECU.batVoltage) & 0x00FF; */
      if (id == 0) {
        /* Serial.println("gear change"); */
        msg[0] = buf[0];
        writeMsg();
      }
    }
  }
  /* delay(75); */
}
