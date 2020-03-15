// CAN CONTROLLER CODE FOR THE DASH

#include "mcp_can.h"
#include <SPI.h>

#define BODY_LENGTH 4
#define BAUD_RATE 1000000

// MOTEC M150 CAN ADDRESS DEFINES
// This short list has only the addresses that are 
#define PDM 0x7F0
#define ECU1 0x118
#define ECU2 0x119
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
    if (id == ECU1) {
      msg[3] = 0x01; // UART MSG ID
      msg[2] = buf[2]; // Speed KM/H
      msg[1] = buf[1]; // TPS
      msg[0] = buf[0]; // RPM
    } else if (id == PDM) {
      msg[3] = 0x03; // UART MSG ID
      msg[2] = buf[2]; // VOLTAGE / 0.1216
      msg[1] = buf[1]; // PDM GLOBAL ERROR
      msg[0] = buf[0]; // FAN STATUS ACTIVE
    } else if (id == ECU2) {
      msg[3] = 0x02; // UART MSG ID
      msg[2] = buf[2]; // VOLTAGE / 0.1216
      msg[1] = buf[1]; // PDM GLOBAL ERROR
      msg[0] = buf[0]; // FAN STATUS ACTIVE
    }
    writeMsg();
    delay(10);
  }
}
