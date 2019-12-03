#include "Arduino.h"
#include <SPI.h>
#include "mcp_can.h"

#include "DEFS.h"

#include "ECU.h"

ECU::ECU() {}
int ECU::begin(int CS_PIN) {
  _CAN = MCP_CAN(CS_PIN);
  int status = _CAN.begin(CAN_250KBPS);
  return status == CAN_OK;
}

void ECU::update() {
  // update ECU and PDM data here
}

