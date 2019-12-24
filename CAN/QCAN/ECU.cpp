#include "Arduino.h"
#include "mcp_can.h"
#include <SPI.h>

#include "DEFS.h"

#include "ECU.h"

ECU::ECU() {}
int ECU::begin(int CS_PIN) {
  _CAN = MCP_CAN(CS_PIN);
  int status = _CAN.begin(CAN_250KBPS);
  return status == CAN_OK;
}

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
    // TODO: update object's public vars with new CAN data
    // develop general function for each type of read (unsigned, signed
    // etc.) See ECU datasheet and old CAN code
    switch (id) {
    case PE1:
      // This message provides RPM, TPS, fuel open time and ignition angle

      break;
    }
  }
}
