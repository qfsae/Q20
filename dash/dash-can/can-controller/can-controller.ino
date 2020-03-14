// CAN CONTROLLER CODE FOR THE DASH

#include "mcp_can.h"
#include <SPI.h>

#define BODY_LENGTH 4
#define BAUD_RATE 1000000

#define SPI_CS_PIN 10

MCP_CAN CAN(SPI_CS_PIN);

unsigned char msg[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Starting");
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");

  // add endmarkers to message
  int endMarker = -20000;
  char *c = (char *)&endMarker;

  msg[4] = c[0];
  msg[5] = c[1];

  msg[0] = 200;
  msg[1] = 200;
}

void writeMsg() {
  size_t idx = 0;
  size_t bytesLeft = 6;
  while (idx != 6) {
    size_t written = Serial.write(&msg[idx], bytesLeft);
    bytesLeft -= written;
    idx += written;
  }
}

void loop() {
  unsigned char len = 0;
  unsigned char buf[8];
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    unsigned long id = CAN.getCanId();
    // for testing sending just battery voltage is fine
    // but more will need to be added
    if (id == 0x118) {
      msg[3]++;
      msg[2] = buf[2];
      msg[1] = buf[1];
      msg[0] = buf[0];
      writeMsg();
      delay(100);
    }
  }
}
