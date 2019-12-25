#include "Arduino.h"
#include "mcp_can.h"
#include <SPI.h>

#include "DEFS.h"

#include "ECU.h"

#define DEBUG_MODE 0

ECU::ECU() {}
void ECU::begin(MCP_CAN CAN) { _CAN = CAN; }

int readUnsigned(unsigned char lowByte, unsigned char highByte) {
  return (highByte * 256) + lowByte;
}

int readSigned(unsigned char lowByte, unsigned char highByte) {
  int reading = (highByte * 256) + lowByte;
  if (reading > 32767) {
    reading -= 65536;
  }
  return reading;
}

float scaleReading(int reading, float factor) { return reading * factor; }

// Updates the public variables on the object when a new message is received
// Running this update function in the loop of an Arduino sketch gives easy
// access to the latest ECU values
void ECU::update() {
  unsigned char msgBuf[8];
  unsigned char msgLen;
  // update ECU and PDM data here
  if (CAN_MSGAVAIL == _CAN.checkReceive()) {
    _CAN.readMsgBuf(&msgLen, msgBuf);
    unsigned long id = _CAN.getCanId();

    if (DEBUG_MODE) {
      Serial.print("Observed ID ");
      Serial.println(id, HEX);
      Serial.print("MSG Length ");
      Serial.println(msgLen);
    }
    // TODO: update object's public vars with new CAN data
    // develop general function for each type of read (unsigned, signed
    // etc.) See ECU datasheet and old CAN code
    switch (id) {
    case PE1:
      // This message provides RPM, TPS, fuel open time and ignition angle
      this->rpm = readUnsigned(msgBuf[0], msgBuf[1]);
      this->tps = scaleReading(readSigned(msgBuf[2], msgBuf[3]), 0.1);
      this->openTime = scaleReading(readSigned(msgBuf[4], msgBuf[5]), 0.1);
      this->ignitionAngle = scaleReading(readSigned(msgBuf[6], msgBuf[7]), 0.1);
      break;
    case PE2:
      this->pressure = scaleReading(readSigned(msgBuf[0], msgBuf[1]), 0.01);
      this->MAP = scaleReading(readSigned(msgBuf[2], msgBuf[3]), 0.01);
      this->lambda = scaleReading(readSigned(msgBuf[4], msgBuf[5]), 0.01);
      this->pressureUnit = readUnsigned(msgBuf[6], msgBuf[7]);
      break;
    case PE3:
      // Provides readings on first 4 analog inputs
      for (int i = 0; i < 8; i += 2) {
        this->analogInputs[i / 2] =
            scaleReading(readSigned(msgBuf[i], msgBuf[i + 1]), 0.001);
      }
      break;
    case PE4:
      // Provides readings for last 4 analog inputs
      for (int i = 0; i < 8; i += 2) {
        this->analogInputs[4 + i / 2] =
            scaleReading(readSigned(msgBuf[i], msgBuf[i + 1]), 0.001);
      }
      break;
    case PE5:
      for (int i = 0; i < 8; i += 2) {
        this->frequencies[i / 2] =
            scaleReading(readSigned(msgBuf[i], msgBuf[i + 1]), 0.2);
      }
      break;
    case PE6:
      break;
    case PE7:
      break;
    }
  }
}
