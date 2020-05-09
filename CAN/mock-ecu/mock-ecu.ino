#include "MOCK.h"
#include "mcp_can.h"

#define SPI_CS_PIN 9

MockECU mock;
MCP_CAN CAN(SPI_CS_PIN);
ECUData input;

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(10000);
  }
  Serial.println("Initialization Success");
  // Populate ECU Data
  input.rpm = 2000;
  // add more test values as needed
}

// send input data on the correct CAN IDs for the PE3 ECU
void loop() { mock.sendData(input, CAN); }
