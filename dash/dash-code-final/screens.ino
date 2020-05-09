void screenUpdate(int sNum) {
  // render the correct screen depending on value from rotary encoder
  switch (sNum) {
    case 0:
      startScreen();
    break;
    case 1:
      RPMScreen(can.rpm);
    break;
    default:
      debugScreen();
    break;
  }
}

// only rendered when CAN data is not being received
void startScreen() {
  GD.Clear();
  GD.Begin(BITMAPS);
  GD.Vertex2ii(240 - 200, 40);
  GD.cmd_text(240, 260, 21, OPT_CENTER, "Waiting for CAN bus");
  GD.End();
  
  GD.cmd_spinner(240, 220, 1, 0);
  
  GD.swap();
}

void RPMScreen(int value) {
  GD.ClearColorRGB(0,0,0);
  GD.Clear();
  GD.cmd_gauge(150, 136, 100, OPT_NOPOINTER, 8, 5, 0, 7500);
  GD.ColorRGB(255,0,0);
  GD.cmd_gauge(150, 136, 100, OPT_NOBACK | OPT_NOTICKS, 14, 5, value, 7500);
  GD.ColorRGB(255,255,255);
//  GD.cmd_text(25, 25, 25, 0, receivedChars);
  GD.cmd_text(130, 240, 24, 0, "RPM");
  GD.cmd_number(150, 180, 24, OPT_CENTER, value);
  GD.cmd_number(350, 136, 31, OPT_CENTER, value); 
  GD.swap();  
}

void debugScreen() {
  // screen that shows raw CAN data and other debug info for the dash and car as a whole  
}
