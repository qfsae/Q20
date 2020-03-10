#include <EEPROM.h>
#include <SPI.h>
#include <GD23Z.h>

#define BODY_LENGTH 4
#define USerial Serial1


struct DashData {
  int rpm;
  unsigned char tps;
  unsigned char gear;
};

unsigned char currentMsg[BODY_LENGTH];
unsigned char data[BODY_LENGTH];
DashData currentData = {.rpm = 0, .tps = 0, .gear = 0};
int n = 0;

void updateDashData(DashData *d) {
  d->rpm = currentMsg[1] * 256 + currentMsg[0];
  d->tps = currentMsg[2];
  d->gear = currentMsg[3]; 
}

void printCurrentMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.print(currentMsg[i]);
    Serial.print('\t');
  }
  Serial.println();
  Serial.println("END");
}
void arrCpy(unsigned char *src, unsigned char *dest) {
  for (int i = 0; i < BODY_LENGTH; i++) {
    dest[i] = src[i];  
  }
}

void recvData() {
  // assign start and end markers for each
//  int startMarker = 20000;
//  char *startBytes = (char *)&startMarker;
  int endMarker = -20000;
  char *endBytes = (char *)&endMarker;

  if (USerial.available() >= 2) {
    Serial.println("RECV");
    char a = USerial.read();
    char b = USerial.read();
    if (a == endBytes[0] && b == endBytes[1]) {
      // The message has ended
      n = 0;
      arrCpy(data, currentMsg);
      // produce DashData struct
      updateDashData(&currentData);
    } else {
      data[n] = a;
      data[n + 1] = b;
      n += 2;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  USerial.begin(115200);
  GD.begin(10);
}

void printDashData() {
  Serial.print("RPM = ");
  Serial.print(currentData.rpm);
  Serial.print(", TPS = ");
  Serial.print(currentData.tps);
  Serial.print(", GEAR = ");
  Serial.print(currentData.gear);
  Serial.print("\n");  
}

void loop() {
  // put your main code here, to run repeatedly:
  recvData();
  //printDashData();
  screenHandler();
  //printCurrentMsg();
}

void screenHandler() {
  int screen = analogRead(A0) / 400;
  if (currentData.rpm < 1000 || currentData.gear > 6 || currentData.gear < 0) {
    return;
  }
switch (screen) {
    case 0:
      // Put the formula logo if you can get the SD card working
      break;
    case 1:
      // Normal Screen
      GD.ClearColorRGB(0, 0, 0);
      GD.Clear();
      GD.cmd_gauge(150, 136, 100, OPT_NOPOINTER, 8, 5, 0, 7500);
      GD.ColorRGB(255, 0, 0);
      GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, currentData.rpm, 7500);
      GD.ColorRGB(255, 255, 255);
      //  GD.cmd_text(25, 25, 25, 0, receivedChars);
      GD.cmd_text(130, 240, 24, 0, "RPM");
      GD.cmd_number(150, 180, 24, OPT_CENTER, currentData.rpm);
      GD.cmd_number(350, 136, 31, OPT_CENTER, currentData.gear);
      GD.swap();
      break;
    case 2:
      // ETC Analytics Screen
      GD.Clear();
      GD.ColorRGB(0, 190, 0);
      GD.ColorRGB(0, 190, 0);
      GD.Begin(RECTS);
      //APPS
      GD.Vertex2ii(59, 41, 0, 0);
      GD.Vertex2ii(142, 130, 0, 0);
      //BRAKE PRESSURE
      GD.Vertex2ii(199, 41, 0, 0);
      GD.Vertex2ii(282, 130, 0, 0);
      //BRAKE SPEED PLAUSIBILITY DEVICE
      GD.Vertex2ii(339, 41, 0, 0);
      GD.Vertex2ii(422, 130, 0, 0);
      //RPM
      GD.Vertex2ii(59, 161, 0, 0);
      GD.Vertex2ii(142, 250, 0, 0);
      //ENGINE TEMPERATURE
      GD.Vertex2ii(199, 161, 0, 0);
      GD.Vertex2ii(282, 250, 0, 0);
      //ERROR
      GD.Vertex2ii(339, 161, 0, 0);
      GD.Vertex2ii(422, 250, 0, 0);
      GD.End();

      GD.ColorRGB(255, 255, 255);
      GD.cmd_text(100, 40, 26, OPT_CENTERX, "Accelerator");
      GD.cmd_text(100, 55, 27, OPT_CENTERX, "Pedal");
      GD.cmd_text(100, 70, 27, OPT_CENTERX, "Position");
      GD.cmd_number(100, 85, 31, OPT_CENTERX, currentData.tps);
      GD.cmd_text(240, 40, 28, OPT_CENTERX, "Brake");
      GD.cmd_text(240, 60, 28, OPT_CENTERX, "Pressure");
      GD.cmd_number(240, 85, 31, OPT_CENTERX, 0);
      GD.cmd_text(380, 40, 27, OPT_CENTERX, "Brake");
      GD.cmd_text(380, 55, 27, OPT_CENTERX, "Speed");
      GD.cmd_text(380, 70, 27, OPT_CENTERX, "Plausibility");
      GD.cmd_number(380, 85, 31,  OPT_CENTERX, 0);
      GD.cmd_text(100, 170, 28, OPT_CENTERX, "RPM");
      GD.cmd_number(100, 200, 31, OPT_CENTERX, currentData.rpm);
      GD.cmd_text(240, 160, 28, OPT_CENTERX, "Engine");
      GD.cmd_text(240, 180, 28, OPT_CENTERX, "Temp");
      GD.cmd_number(240, 200, 31, OPT_CENTERX, 0);
      GD.cmd_text(380, 170, 28, OPT_CENTERX, "Error");
      GD.cmd_number(380, 200, 30, OPT_CENTERX, 0);

      GD.swap();
      break;
    default:
      break;
  }
}
