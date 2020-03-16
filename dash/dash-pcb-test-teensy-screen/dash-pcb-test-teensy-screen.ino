#include <Time.h>
#include <TimeLib.h>

#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define BAUD_RATE 1000000
#define USerial Serial1
#define BODY_LENGTH 4
#define ENCODED_LENGTH BODY_LENGTH + 2

IntervalTimer timer;

volatile unsigned char n = 0;
volatile uint8_t currentMsg[BODY_LENGTH];
volatile uint8_t data[ENCODED_LENGTH];
volatile int tps;

bool canStarted = false;

void timerInterrupt() {
  recvData();  
}

volatile uint8_t fcobs_decode(volatile uint8_t *data, volatile uint8_t *dest, volatile uint8_t data_len) {
  const volatile uint8_t *start = dest, *end = data + data_len;
  volatile uint8_t code = 0xFF, copy = 0;

  for (; data < end; copy--) {
    if (copy != 0) {
      *dest++ = *data++;
    } else {
      if (code != 0xFF)
        *dest++ = 0;
      copy = code = *data++;
      if (code == 0)
        break; /* Source length too long */
    }
  }
  return dest - start;
}

void recvData() {
  int endMarker = -20000;
  char *endBytes = (char *)&endMarker;
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

void screenUpdate() {
  GD.ClearColorRGB(0,0,0);
  GD.Clear();
  GD.cmd_gauge(150, 136, 100, OPT_NOPOINTER, 8, 5, 0, 7500);
  GD.ColorRGB(255,0,0);
  GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, tps, 7500);
  GD.ColorRGB(255,255,255);
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
    //recvData();
  } else {
    startScreen();
    if (USerial.available()) {
      canStarted = true;
    }  
  }
}
