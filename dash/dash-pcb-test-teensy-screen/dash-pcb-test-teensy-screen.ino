#include <Time.h>
#include <TimeLib.h>

#include <EEPROM.h>
#include <GD2.h>
#include <SPI.h>

#define BAUD_RATE 1000000
#define USerial Serial1
#define BODY_LENGTH 4

IntervalTimer timer;

volatile unsigned char n = 0;
volatile unsigned char currentMsg[BODY_LENGTH];
volatile unsigned char data[BODY_LENGTH + 2];
volatile int tps;

bool canStarted = false;

void arrCpy(unsigned char *src, unsigned char *dest) {
  for (int i = 0; i < BODY_LENGTH; i++) {
    dest[i] = src[i];
  }
}

void timerInterrupt() { recvData(); }

void recvData() {
  if (USerial.available()) {
    uint8_t a = USerial.read();
    if (a == 0) {
      n = 0;
      fcobs_decode(data, currentMsg, BODY_LENGTH + 2);
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

void screenUpdate() {
  GD.ClearColorRGB(0, 0, 0);
  GD.Clear();
  GD.cmd_gauge(150, 136, 100, OPT_NOPOINTER, 8, 5, 0, 7500);
  GD.ColorRGB(255, 0, 0);
  GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, tps, 7500);
  GD.ColorRGB(255, 255, 255);
  //  GD.cmd_text(25, 25, 25, 0, receivedChars);
  GD.cmd_text(130, 240, 24, 0, "RPM");
  GD.cmd_number(150, 180, 24, OPT_CENTER, tps);
  GD.cmd_number(350, 136, 31, OPT_CENTER, tps);
  GD.swap();
}

void startScreen() {
  GD.Clear();
  GD.Begin(BITMAPS);
  GD.Vertex2ii(240 - 200, 40);
  GD.cmd_text(240, 260, 21, OPT_CENTER, "Waiting for CAN bus");
  GD.End();

  GD.cmd_spinner(240, 220, 1, 0);

  GD.swap();
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
