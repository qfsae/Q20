#include "Arduino.h"
#include "mcp_can.h"
#include <SPI.h>

#include "DEFS.h"

#include "ECU.h"

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

void ECU::update() {
  unsigned char msgBuf[8];
  unsigned char msgLen;
  // update ECU and PDM data here
  if (CAN_MSGAVAIL == _CAN.checkReceive()) {
    _CAN.readMsgBuf(&msgLen, msgBuf);
    unsigned long id = _CAN.getCanId();
    Serial.print("Observed ID ");
    Serial.println(id, HEX);
    // TODO: update object's public vars with new CAN data
    // develop general function for each type of read (unsigned, signed
    // etc.) See ECU datasheet and old CAN code
    switch (id) {
    case PE1:
      // This message provides RPM, TPS, fuel open time and ignition angle
      this->rpm = readUnsigned(msgBuf[0], msgBuf[1]);
      this->tps = readSigned(msgBuf[2], msgBuf[3]);
      break;
    }
  }
}
