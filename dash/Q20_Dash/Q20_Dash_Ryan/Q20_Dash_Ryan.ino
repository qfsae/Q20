#include <GD23Z.h>
#include <SPI.h>

void setup() {
  // put your setup code here, to run once:
  GD.begin(0);
}

void driveScreen() {
  int pot = analogRead(A4) - 924;
  // put your main code here, to run repeatedly:
  GD.cmd_gauge(117, 137, 130, OPT_FLAT | OPT_NOBACK | OPT_NOPOINTER, 4, 8, 0,
               100);
  GD.ColorRGB(255, 0, 0);
  GD.cmd_gauge(117, 137, 130, OPT_FLAT | OPT_NOBACK | OPT_NOTICKS, 4, 8, pot,
               110);
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
  GD.ColorRGB(0, 190, 0);
  GD.Begin(RECTS);
  // PDM
  GD.Vertex2ii(232, 31, 0, 0);
  GD.Vertex2ii(315, 96, 0, 0);
  // FAN
  GD.Vertex2ii(392, 31, 0, 0);
  GD.Vertex2ii(476, 96, 0, 0);
  // ENGINE TEMP
  GD.Vertex2ii(232, 108, 0, 0);
  GD.Vertex2ii(315, 173, 0, 0);
  // OIL PRESSURE
  GD.Vertex2ii(393, 108, 0, 0);
  GD.Vertex2ii(476, 173, 0, 0);
  // OIL TEMPERATURE
  GD.Vertex2ii(232, 192, 0, 0);
  GD.Vertex2ii(315, 258, 0, 0);
  // BATTERY VOLTAGE
  GD.Vertex2ii(393, 192, 0, 0);
  GD.Vertex2ii(476, 258, 0, 0);
  GD.End();

  GD.ColorRGB(0, 0, 0);
  GD.cmd_text(7, 0, 27, 0, "D");
  GD.ColorRGB(255, 255, 255);
  GD.cmd_text(273, 108, 28, OPT_CENTERX, "ETemp");
  GD.cmd_number(273, 127, 31, OPT_CENTERX, 115);
  GD.cmd_text(273, 192, 28, OPT_CENTERX, "OTemp");
  GD.cmd_number(273, 212, 31, OPT_CENTERX, 110);
  GD.cmd_text(434, 108, 28, OPT_CENTERX, "OPres");
  GD.cmd_number(434, 128, 31, OPT_CENTERX, 70);
  GD.cmd_text(434, 192, 28, OPT_CENTERX, "BVolt");
  GD.cmd_text(434, 212, 31, OPT_CENTERX, "13.2");
  GD.cmd_text(273, 30, 28, OPT_CENTERX, "PDM");
  GD.cmd_text(273, 48, 31, OPT_CENTERX, "OK");
  GD.cmd_text(434, 30, 28, OPT_CENTERX, "FAN");
  GD.cmd_text(434, 48, 31, OPT_CENTERX, "ON");
  GD.cmd_number(120, 204, 31, OPT_CENTERX, pot);
}

void etc_Screen() {
  GD.ColorRGB(75, 75, 75);
  GD.Begin(EDGE_STRIP_A);
  GD.Vertex2ii(0, 20, 0, 0);
  GD.Vertex2ii(480, 20, 0, 0);
  GD.End();
  // makes the top gray strip
  GD.ColorRGB(0, 0, 0);
  GD.cmd_text(7, 0, 27, 0, "ETC");
  // identifies the screen as being the ETC display info screen
  GD.ColorRGB(0, 190, 0);
  GD.ColorRGB(0, 190, 0);
  GD.Begin(RECTS);
  // APPS
  GD.Vertex2ii(59, 41, 0, 0);
  GD.Vertex2ii(142, 130, 0, 0);
  // BRAKE PRESSURE
  GD.Vertex2ii(199, 41, 0, 0);
  GD.Vertex2ii(282, 130, 0, 0);
  // BRAKE SPEED PLAUSIBILITY DEVICE
  GD.Vertex2ii(339, 41, 0, 0);
  GD.Vertex2ii(422, 130, 0, 0);
  // RPM
  GD.Vertex2ii(59, 161, 0, 0);
  GD.Vertex2ii(142, 250, 0, 0);
  // ENGINE TEMPERATURE
  GD.Vertex2ii(199, 161, 0, 0);
  GD.Vertex2ii(282, 250, 0, 0);
  // ERROR
  GD.Vertex2ii(339, 161, 0, 0);
  GD.Vertex2ii(422, 250, 0, 0);
  GD.End();

  GD.ColorRGB(255, 255, 255);
  GD.cmd_text(100, 40, 26, OPT_CENTERX, "Accelerator");
  GD.cmd_text(100, 55, 27, OPT_CENTERX, "Pedal");
  GD.cmd_text(100, 70, 27, OPT_CENTERX, "Position");
  GD.cmd_number(100, 85, 31, OPT_CENTERX, 0);
  GD.cmd_text(240, 40, 28, OPT_CENTERX, "Brake");
  GD.cmd_text(240, 60, 28, OPT_CENTERX, "Pressure");
  GD.cmd_number(240, 85, 31, OPT_CENTERX, 0);
  GD.cmd_text(380, 40, 27, OPT_CENTERX, "Brake");
  GD.cmd_text(380, 55, 27, OPT_CENTERX, "Speed");
  GD.cmd_text(380, 70, 27, OPT_CENTERX, "Plausibility");
  GD.cmd_number(380, 85, 31, OPT_CENTERX, 0);
  GD.cmd_text(100, 170, 28, OPT_CENTERX, "RPM");
  GD.cmd_number(100, 200, 31, OPT_CENTERX, 0);
  GD.cmd_text(240, 160, 28, OPT_CENTERX, "Engine");
  GD.cmd_text(240, 180, 28, OPT_CENTERX, "Temp");
  GD.cmd_number(240, 200, 31, OPT_CENTERX, 0);
  GD.cmd_text(380, 170, 28, OPT_CENTERX, "Error");
  GD.cmd_number(380, 200, 30, OPT_CENTERX, 0);
}

void loop() {
  GD.ClearColorRGB(0, 0, 0);
  GD.Clear();
  driveScreen();
  // etc_Screen();

  Serial.println(analogRead(A4));
  //  for(int i =0; i>0; i++)
  //  {
  //
  //  }
  GD.swap();
}
