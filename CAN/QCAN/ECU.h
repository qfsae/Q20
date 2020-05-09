#ifndef ECU_h
#define ECU_h

#include "Arduino.h"

class ECU {
public:
  ECU();
  void update(unsigned long id, unsigned char msgBuf[], unsigned char msgLen);
  void debugPrint(unsigned long id);
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

#endif
