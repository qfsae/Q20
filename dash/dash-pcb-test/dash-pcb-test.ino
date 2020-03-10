#include "mcp_can.h"

#define ADDR 0x00
#define BODY_LENGTH 4
#define SPI_CS_PIN 10

struct DashData {
  int rpm;
  unsigned char tps;
  unsigned char gear;
  unsigned char *rawMsg;
};

MCP_CAN CAN(SPI_CS_PIN);

unsigned char msg[6] = {0, 0, 0, 0, 0, 0};

// given a message buffer from the CAN sim data produce a DashData type
// to send via UART
DashData fetchCanDashData(unsigned char msgBuf[]) {
  DashData returnData;
  returnData.rpm = msgBuf[1] * 256 + msgBuf[0];
  returnData.tps = msgBuf[2];
  returnData.gear = msgBuf[3];
  returnData.rawMsg = msgBuf;
  return returnData;
}

void writeDashData(DashData d) {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.write(d.rawMsg[i]);
  }
  int endMarker = -20000;
  char *endBytes = (char *)&endMarker;
  Serial.write(endBytes[0]);
  Serial.write(endBytes[1]);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  while (CAN.begin(CAN_250KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");
  delay(500);
}

void loop() {
  unsigned char len = 0;
  unsigned char buf[8];
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    unsigned long id = CAN.getCanId();
    if (id == ADDR) {
      for (int i = 0; i < len; i++) {
        Serial.write(buf[i]);  
      }
      int endMarker = -20000;
      char *endBytes = (char *)&endMarker;
      Serial.write(endBytes[0]);
      Serial.write(endBytes[1]);
    }
  }
}
