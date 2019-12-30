#include "MOCK.h"
#include "Arduino.h"
#include "DEFS.h"
#include "mcp_can.h"

MockECU::MockECU() {}

/*
  The ECU sends integers in blocks of two bytes. This function maps an input
  integer a to an array of bytes and loads them into the message buffer in the
  location indicated by the startIndex parameter.
*/
void byteConv(int *a, unsigned char buf[], int startIndex) {
  char *b = (char *)a;
  buf[startIndex] = b[0];
  buf[startIndex + 1] = b[1];
}

/*
  Populate this function after it is confirmed that the ECU testbench works.
*/
void MockECU::sendData(ECUData d, MCP_CAN C) {}
