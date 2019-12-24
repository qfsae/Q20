#ifndef ECU_h
#define ECU_h

#include "Arduino.h"
#include "mcp_can.h"
#include <SPI.h>

class ECU {
public:
  ECU();
  int begin(int CS_PIN);
  void update();
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
  // TODO: Add variables for page 2 of ECU Datasheet

private:
  int _CS_PIN;
  MCP_CAN _CAN;
};

#endif
