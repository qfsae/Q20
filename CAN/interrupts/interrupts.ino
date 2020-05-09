#include "mcp_can.h"
#define SPI_CS_PIN 9
#define SENDING 0
#define ADDR 0x00

volatile int gear = 1;
volatile bool interrupted = false;

MCP_CAN CAN(SPI_CS_PIN);
unsigned char len = 8;
unsigned char message[8];

void downShift() {
  if (gear > 1) {
    gear--;
  }
  message[0] = gear;
  if (SENDING)
    CAN.sendMsgBuf(ADDR, 0, 8, message);
}

void upShift() {
  if (gear < 6) {
    gear++;
  }
  message[0] = gear;
  if (SENDING)
    CAN.sendMsgBuf(ADDR, 0, 8, message);
}

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(10000);
  }
  Serial.println("Initialization Success");
  attachInterrupt(digitalPinToInterrupt(2), downShift, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), upShift, FALLING);
}

void loop() {
  if (!SENDING && CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, message);
    unsigned long id = CAN.getCanId();
    Serial.print("Getting Data from ID: ");
    Serial.println(id, HEX);
    Serial.print("gear = ");
    Serial.println(message[0]);
  }
}
