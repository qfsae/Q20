#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#define BAUD_RATE 1000000
#define USerial Serial1
#define BODY_LENGTH 4

unsigned char n = 0;
unsigned char currentMsg[BODY_LENGTH];
unsigned char data[BODY_LENGTH];

bool canStarted = false;
int tps = 0;

void arrCpy(unsigned char *src, unsigned char *dest) {
  for (int i = 0; i < BODY_LENGTH; i++) {
    dest[i] = src[i];  
  }
}

void recvData() {
  int endMarker = -20000;
  char *endBytes = (char *)&endMarker;
  //Serial.println("RUN");
  if (USerial.available() >= 2) {
    //Serial.println("RECV");
    char a = USerial.read();
    char b = USerial.read();
    if (a == endBytes[0] && b == endBytes[1]) {
      // The message has ended
      n = 0;
      //arrCpy(data, currentMsg);
      if (data[3] == 0x01) {
        screenUpdate();
        //tps = data[1];
      }
    } else {
      data[n] = a;
      data[n + 1] = b;
      n += 2;
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
  GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, data[1], 7500);
  GD.ColorRGB(255,255,255);
//  GD.cmd_text(25, 25, 25, 0, receivedChars);
  GD.cmd_text(130, 240, 24, 0, "RPM");
  GD.cmd_number(150, 180, 24, OPT_CENTER, data[1]);
  GD.cmd_number(350, 136, 31, OPT_CENTER, data[1]); 
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
  //while (!USerial.available());
}

void loop() {
  // put your main code here, to run repeatedly:
  if (canStarted) {
    recvData();  
  } else {
    startScreen();
    if (USerial.available()) {
      canStarted = true;
    }  
  }
}
