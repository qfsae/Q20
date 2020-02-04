#define CS_PIN 10

#include <EEPROM.h>
#include <SPI.h>
#include <GD23Z.h>

// Length of UART messages in bytes
// must always be an even number so that start and end markers
// can be decoded properly
#define BODY_LENGTH 4

// CAN
#include "mcp_can.h"

#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);

// UART Config
#define USerial Serial1

int rpm;
int gear;

unsigned char n = 0;
unsigned char currentMsg[BODY_LENGTH];
unsigned char data[BODY_LENGTH];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  USerial.begin(115200);
  
  GD.begin(CS_PIN);
  delay(500);
  
  endian();
}

<<<<<<< HEAD
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
    } else {
      data[n] = a;
      data[n + 1] = b;
      n += 2;
    }
  }
}

void endian() {
  unsigned int i = 1;  
  char *c = (char*)&i;  
  if (*c)
    Serial.println("Little Endian");  
  else
    Serial.println("Big Endian");     
}

void printCurrentMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.print(currentMsg[i]);
    Serial.print('\t');
  }
  Serial.println();
  Serial.println("END");
}

void driveScreen()
{
  int pot = currentMsg[0];//analogRead(A4)-924;
  // put your main code here, to run repeatedly:
  GD.cmd_gauge(117, 137, 130, OPT_FLAT | OPT_NOBACK | OPT_NOPOINTER, 4, 8, 0, 100);
  GD.ColorRGB(255,0,0);
  GD.cmd_gauge(117, 137, 130, OPT_FLAT | OPT_NOBACK | OPT_NOTICKS, 4, 8, pot, 110);
  GD.Begin(LINES);
  GD.Vertex2ii(209, 155, 0, 0);
  GD.Vertex2ii(218, 157, 0, 0);
  GD.Vertex2ii(206, 168, 0, 0);
  GD.Vertex2ii(214, 172, 0, 0);
  GD.Vertex2ii(201, 181, 0, 0);
  GD.Vertex2ii(207, 185, 0, 0);
  GD.Vertex2ii(193, 193, 0, 0);
  GD.Vertex2ii(200, 199, 0, 0);
  GD.End();
  GD.PointSize(60);
  GD.Begin(POINTS);
  GD.Vertex2ii(182, 201, 0, 0);
  GD.Vertex2ii(185, 204, 0, 0);
  GD.Vertex2ii(187, 206, 0, 0);
  GD.Vertex2ii(189, 208, 0, 0);
  GD.End();

  GD.ColorRGB(75, 75, 75);
  GD.Begin(EDGE_STRIP_A);
  GD.Vertex2ii(0, 20, 0, 0);
  GD.Vertex2ii(480, 20, 0, 0);
  GD.End();
  GD.ColorRGB(0,190,0);
  GD.Begin(RECTS);
  //PDM
  GD.Vertex2ii(232, 31, 0, 0);
  GD.Vertex2ii(315, 96, 0, 0);
  //FAN
  GD.Vertex2ii(392, 31, 0, 0);
  GD.Vertex2ii(476, 96, 0, 0);
  //ENGINE TEMP
  GD.Vertex2ii(232, 108, 0, 0);
  GD.Vertex2ii(315, 173, 0, 0);
  //OIL PRESSURE
  GD.Vertex2ii(393, 108, 0, 0);
  GD.Vertex2ii(476, 173, 0, 0);
  //OIL TEMPERATURE
  GD.Vertex2ii(232, 192, 0, 0);
  GD.Vertex2ii(315, 258, 0, 0);
  //BATTERY VOLTAGE
  GD.Vertex2ii(393, 192, 0, 0);
  GD.Vertex2ii(476, 258, 0, 0);
  GD.End();


  GD.ColorRGB(0, 0, 0);
  GD.cmd_text(7, 0, 27, 0, "D");
  GD.ColorRGB(255,255,255);
  GD.cmd_text(273, 108, 28, OPT_CENTERX, "ETemp");
  GD.cmd_number(273, 127, 31, OPT_CENTERX, 115);
  GD.cmd_text(273, 192, 28, OPT_CENTERX, "OTemp");
  GD.cmd_number(273, 212, 31, OPT_CENTERX, 110);
  GD.cmd_text(434, 108, 28, OPT_CENTERX, "OPres");
  GD.cmd_number(434, 128, 31, OPT_CENTERX, 70);
  GD.cmd_text(434, 192, 28, OPT_CENTERX, "BVolt");
  GD.cmd_number(434, 212, 31, OPT_CENTERX, currentMsg[1]);
  GD.cmd_text(273, 30, 28, OPT_CENTERX, "PDM");
  GD.cmd_text(273, 48, 31, OPT_CENTERX, "OK");
  GD.cmd_text(434, 30, 28, OPT_CENTERX, "FAN");
  GD.cmd_text(434, 48, 31, OPT_CENTERX, "ON");
  GD.cmd_number(120, 204, 31, OPT_CENTERX, rpm);
}

void loop() {
  recvData();
  printCurrentMsg();
  rpm = currentMsg[0];
  GD.ClearColorRGB(0,0,0);
  GD.Clear();
  driveScreen();
  GD.swap();
=======
void loop() {
    unsigned char buf = 0;
    if (USerial.available() > 0) {
      buf = USerial.read();
      Serial.println(buf);
      rpm = buf;
    }
    GD.ClearColorRGB(0,0,0);
    GD.Clear();
    GD.cmd_gauge(150, 136, 100, OPT_NOPOINTER, 8, 5, 0, 7500);
    GD.ColorRGB(255,0,0);
    GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, rpm, 7500);
    GD.ColorRGB(255,255,255);
  //  GD.cmd_text(25, 25, 25, 0, receivedChars);
    GD.cmd_text(130, 240, 24, 0, "RPM");
    GD.cmd_number(150, 180, 24, OPT_CENTER, rpm);
    GD.cmd_number(350, 136, 31, OPT_CENTER, gear);
    GD.swap();
>>>>>>> origin/master
}
