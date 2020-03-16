#include <Time.h>
#include <TimeLib.h>

#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define BAUD_RATE 1000000
#define USerial Serial1
#define BODY_LENGTH 4
#define ENCODED_LENGTH BODY_LENGTH + 2

struct CANData {
  int rpm;
  uint8_t tps;
  float voltage;
  int speed; // in km/h
  bool pdmError;
  bool fanActive;
};

IntervalTimer timer;

volatile unsigned char n = 0;
volatile uint8_t currentMsg[BODY_LENGTH];
volatile uint8_t data[ENCODED_LENGTH];
volatile int tps;

volatile bool canStarted = false;

void timerInterrupt() {
  recvData();  
}

void recvData() {
  if (USerial.available()) {
    char a = USerial.read();
    if (a == 0) {
      // The message has ended
      fcobs_decode(data, currentMsg, ENCODED_LENGTH);
      n = 0;
      if (currentMsg[3] == 0x01) {
        tps = currentMsg[1];
      }
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
  timer.priority(0); // set highest priority for this interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  if (canStarted) {
    screenUpdate();
  } else {
    startScreen();
    if (USerial.available()) {
      canStarted = true;
    }
  }
}
