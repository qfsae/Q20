#include <Time.h>
#include <TimeLib.h>

#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define BAUD_RATE 1000000
#define USerial Serial1

#define BODY_LENGTH 4
#define ENCODED_LENGTH BODY_LENGTH + 2

#define INTERRUPT_PRIORITY 0

// the factor voltage values from CAN must be scaled by
#define VOLTAGE_FACTOR 0.1216

struct CANData {
  int rpm;
  uint8_t tps;
  float voltage;
  int speed; // in km/h
  bool pdmError;
  bool fanActive;
  uint8_t fuelTemp;
  uint8_t oilTemp;
  uint8_t coolantTemp;
  uint8_t differentialTemp;
  uint8_t transmissionTemp;
  int fuelPressure; // in delta kpa
  int steeringAngle; // in degrees
};

IntervalTimer timer;

volatile unsigned char n = 0;
volatile uint8_t currentMsg[BODY_LENGTH];
volatile uint8_t data[ENCODED_LENGTH];

volatile struct CANData can;
volatile uint8_t sNumber = 0;
volatile bool canStarted;

// takes the UART ID of the packet and updates the related fields in the struct
void updateCANData(uint8_t idUART) {
  switch (idUART) {
    case 0x10:
      can.speed = currentMsg[2];
      can.tps = currentMsg[1];
      can.rpm = currentMsg[0] * 100;
    break;
    case 0x20:
      can.fuelTemp = currentMsg[2];
      can.oilTemp = currentMsg[1];
      can.coolantTemp = currentMsg[0];
    break;
    case 0x30:
      can.differentialTemp = currentMsg[2];
      can.transmissionTemp = currentMsg[1];
    break;
    case 0x02:
      can.fuelPressure = currentMsg[0] * 10;
      can.steeringAngle = currentMsg[2] * 256 + currentMsg[1];
    break;
    case 0x03:
      can.voltage = (float)currentMsg[2] * VOLTAGE_FACTOR;
      can.pdmError = currentMsg[1];
      can.fanActive = currentMsg[0];
    break;
  }
}

void timerInterrupt() {
  recvData();
}

void recvData() {
  if (USerial.available()) {
    uint8_t a = USerial.read();
    if (a == 0) {
      // The message has ended
      fcobs_decode(data, currentMsg, ENCODED_LENGTH);
      updateCANData(currentMsg[3]);
      n = 0;
    } else {
      data[n] = a;
      n++;
    }
  }
}

void printCurrentMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.print(currentMsg[i]);
    Serial.print('\t');
  }
  Serial.println();
  Serial.println("END");  
}

void setup() {
  // put your setup code here, to run once:
  USerial.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);
  delay(500);
  GD.begin();
  GD.cmd_loadimage(0, 0);
  GD.load("qfsae.jpg");
  Serial.println("STARTING");
  timer.begin(timerInterrupt, 100);
  timer.priority(INTERRUPT_PRIORITY); // set highest priority for this interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  if (USerial.available() && !canStarted) {
    canStarted = true;
    sNumber = 1;
  }
  screenUpdate(sNumber);
}
