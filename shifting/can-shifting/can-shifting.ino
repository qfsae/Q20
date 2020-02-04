#include <SPI.h>
#include <mcp_can.h>
#include <mcp_can_dfs.h>

// use highest priority CAN address for transmitting shift events
#define ADDR 0x00
#define MSG_LEN 8

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 53;
const int downBut = 18; // Button from Paddle Shifts
const int upBut = 19;

const int upSol = 27;   // Signal to upshift solenoid
const int downSol = 26; // Signal to downshift solenoid

MCP_CAN CAN(SPI_CS_PIN); // Set CS pin

volatile uint8_t gear = 0;
volatile unsigned char message[MSG_LEN] = {0, 0, 0, 0, 0, 0, 0, 0};

void upShift() {
  if (gear < 6)
    gear++;
  Serial.println(gear);
  message[0] = gear;
  CAN.sendMsgBuf(ADDR, 0, MSG_LEN, message);
}

void downShift() {
  if (gear > 1)
    gear--;
  Serial.println(gear);
  message[0] = gear;
  CAN.sendMsgBuf(ADDR, 0, MSG_LEN, message);
}

void setup() {
  Serial.begin(115200);
  pinMode(downBut, INPUT);
  pinMode(upBut, INPUT);
  pinMode(upSol, OUTPUT);
  pinMode(downSol, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(upBut), upShift, FALLING);
  attachInterrupt(digitalPinToInterrupt(downBut), downShift, FALLING);

  while (CAN_OK != CAN.begin(CAN_250KBPS)) // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(1);
  }
  Serial.println("CAN BUS Shield init ok!");
}

void loop() {}
