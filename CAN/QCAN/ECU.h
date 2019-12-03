#ifndef ECU_h
#define ECU_h

#include "Arduino.h"
#include <SPI.h>
#include "mcp_can.h"

class ECU {
 public:
  ECU();
  int begin(int CS_PIN);
  void update();
  int rpm;
  int tps;
 private:
  int _CS_PIN;
  MCP_CAN _CAN;
};

#endif
