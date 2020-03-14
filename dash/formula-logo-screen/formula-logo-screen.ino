#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

void setup()
{
  GD.begin();
  GD.cmd_loadimage(0, 0);
  GD.load("qfsae.jpg");
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

void loop()
{
  startScreen();
}
