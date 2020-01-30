/* This file will be the code for the teensy 

- Wire potentiometer with screen
- On one screen, logo?
- On one screen, RPM gauge with gear and tps
- On another screen, APPS/TPS, error, brake pressure? -> Need to have CAN developed between ETC and Dash
*/

#define CS_PIN 10 // dp we need this ?
#include <EEPROM.h>
#include <SPI.h>
#include <GD23Z.h>

#define screenToggle A0;

// UART Config
#define USerial Serial1

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

char buf[2];
int rpm;
int gear;
int lastGear = 0;
float tps;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false; 

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    USerial.begin(115200);
    pinMode(screenToggle, INPUT);
    GD.begin(CS_PIN);
    startMillis = millis();  //initial start time
}

void loop() {
    int screen = getScreen();
    unsigned char buf = 0;
    /* This will have to be revamped to accept etc related stuff and everything from the sim */
    if (USerial.available() > 0) {
      buf = USerial.read();
      Serial.println(buf);
      rpm = buf;
    }
    switch(screen){
        case 0:
            // Put the formula logo if you can get the SD card working
            break;
        case 1:
            // Normal Screen
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
            break;
        case 2:
            // ETC Analytics Screen
            break;
        default:
            break;
    }

}

int getScreen(){
    return analogRead(screenToggle)/400; // this should give us 3 different screens
}