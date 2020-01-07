#ifndef MOCK_h
#define MOCK_h

#include "Arduino.h"
#include "mcp_can.h"

struct ECUData {
  int rpm;
  float tps;
  float openTime;
  float ignitionAngle;
  float pressure;
  bool pressureUnit;
  float MAP;
  float lambda;
  float analogInputs[8];
  float frequencies[4];
  float batVoltage;
  float airTemp;
  float coolantTemp;
  bool tempUnit;
  float thermistors[2];
  unsigned char versionMajor;
  unsigned char versionMinor;
  unsigned char versionBuild;
};

class MockECU {
public:
  MockECU();
  void sendData(ECUData d, MCP_CAN C);
};

#endif
