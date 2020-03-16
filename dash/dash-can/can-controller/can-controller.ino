// CAN CONTROLLER CODE FOR THE DASH
// By: Ethan Peterson
// March 16, 2020

#include "mcp_can.h"
#include <SPI.h>
#include "fcobs.h"

// number of bytes sent from PDM CAN Messages
#define PDM_BODY_LENGTH 4
#define PDM_BODY_LENGTH_ENCODED PDM_BODY_LENGTH + 2

// number of bytes sent from ECU CAN Messages
#define ECU_BODY_LENGTH 4
#define ECU_BODY_LENGTH_ENCODED ECU_BODY_LENGTH + 2

#define BAUD_RATE 1000000

// MOTEC M150 CAN ADDRESS DEFINES
// This short list has only the addresses that are
#define PDM 0x7F0
#define ECU1 0x118
#define ECU2 0x119

#define SPI_CS_PIN 10

MCP_CAN CAN(SPI_CS_PIN);

uint8_t msgPDM[PDM_BODY_LENGTH] = {0, 0, 0, 0};
uint8_t encodedPDM[PDM_BODY_LENGTH + 2];

uint8_t msgECU[ECU_BODY_LENGTH];
uint8_t encodedECU[ECU_BODY_LENGTH_ENCODED];

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Starting");
  while (CAN.begin(CAN_1000KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");
}

void writeMsg(uint8_t *buf, uint8_t len) {
  size_t idx = 0;
  size_t bytesLeft = len;
  while (idx != len) {
    size_t written = Serial.write(&buf[idx], bytesLeft);
    bytesLeft -= written;
    idx += written;
  }
}

void sendECUMessage(uint8_t *buf) {
  msgECU[3] = 0x10;   // UART MSG ID
  msgECU[2] = buf[2]; // Speed KM/H
  msgECU[1] = buf[1]; // TPS
  msgECU[0] = buf[0]; // RPM

  // encode first chunk of data
  fcobs_encode(msgECU, encodedECU, ECU_BODY_LENGTH);
  writeMsg(encodedECU, ECU_BODY_LENGTH_ENCODED);

  msgECU[3] = 0x20;
  msgECU[2] = buf[5]; // fuel temp
  msgECU[1] = buf[4]; // oil temp
  msgECU[0] = buf[3]; // coolant temp

  fcobs_encode(msgECU, encodedECU, ECU_BODY_LENGTH);
  writeMsg(encodedECU, ECU_BODY_LENGTH_ENCODED);

  msgECU[3] = 0x30;
  msgECU[2] = buf[7]; // differential temp
  msgECU[1] = buf[6]; // transmission temp
  msgECU[0] = 0; // not used
  
  fcobs_encode(msgECU, encodedECU, ECU_BODY_LENGTH);
  writeMsg(encodedECU, ECU_BODY_LENGTH_ENCODED);
}

void loop() {
  unsigned char len = 0;
  unsigned char buf[8];
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    unsigned long id = CAN.getCanId();
  
    if (id == ECU1) {
      sendECUMessage(buf);
    } else if (id == PDM) {
      msgPDM[3] = 0x03;   // UART MSG ID
      msgPDM[2] = buf[2]; // VOLTAGE / 0.1216
      msgPDM[1] = buf[1]; // PDM GLOBAL ERROR
      msgPDM[0] = buf[0]; // FAN STATUS ACTIVE
      fcobs_encode(msgPDM, encodedPDM, PDM_BODY_LENGTH);
      writeMsg(encodedPDM, PDM_BODY_LENGTH_ENCODED);
    } else if (id == ECU2) {
      msgPDM[3] = 0x02;   // UART MSG ID
      msgPDM[0] = buf[0]; // fuel pressure / 10 in delta kpa
      msgPDM[1] = buf[2]; // LSB of steering angle
      msgPDM[2] = buf[3]; // MSB of steering angle
      fcobs_encode(msgPDM, encodedPDM, PDM_BODY_LENGTH);
      writeMsg(encodedPDM, PDM_BODY_LENGTH_ENCODED);
    }
  }
}
