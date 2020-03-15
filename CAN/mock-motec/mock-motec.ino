#include "mcp_can.h"
#include <SPI.h>

#define BAUD_RATE 1000000

// MOTEC M150 CAN ADDRESS DEFINES
// This short list has only the addresses that are 
#define PDM 0x7F0
#define ECU1 0x118
#define ECU2 0x119

// 10 for can bus shield version 1.2
#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);

uint8_t sendBuffer[8];

void startCAN() {
  Serial.println("Starting");
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  startCAN();
}

void clearArr() {
  for (int i = 0; i < 8; i++) {
    sendBuffer[i] = 0;
  }
}

uint8_t rpm = 0;
uint8_t tps = 0;
void loop() {
  // put your main code here, to run repeatedly:
  
  // Send RPM and TPS
  if (tps > 100) { // bound tps from 0 - 100% throttle
    tps = 0;
  }

  sendBuffer[0] = rpm++;
  sendBuffer[1] = tps++;
  CAN.sendMsgBuf(ECU1, 0, 8, sendBuffer);
  clearArr();
  //delay(25);
  // Send Battery Voltage
  sendBuffer[2] = 100; // pos 2 in the buffer is the battery voltage
  CAN.sendMsgBuf(PDM, 0, 8, sendBuffer);
  clearArr();
  // Send Engine Temp (M150 not currently sending this?)
  // Send Oil Pressure ^
  // Send Brake Pressure ^

}
