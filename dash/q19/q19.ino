/* This is the latest version of the q19 dash system
- Last updates were made on September 11th, 2019
- Updates include:
  - Refactored code, made it more organized
  - Added more modes for the toggle
  - Updated CAN init method
  - Added more cool startup sequence stuff
*/

// CAN Libraries 
#include <mcp_can.h>
#include <mcp_can_dfs.h>
// using latest CAN library from
// https://github.com/Seeed-Studio/CAN_BUS_Shield
// as of April 5, 2019

#include <SPI.h>
#include <Wire.h>

// OLED Screen Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// LED Flashing
// RPM
long rpmFlashLast = 0;
byte rpmFlash = 1;
// Engine Temp
long engTempFlashLast = 0;
byte engTempFlash = 1;
// GTFO
long gtfoFlashLast = 0;
byte gtfoFlash = 1;
int currentCycle = 0;
// Shift Registers 
#define latchPIN    4 // pin 3 on ATMega328P
#define clockPIN    7 // pin 11 on ATMega328P
#define dataPIN     3 // pin 1 on ATMega328P
#define pinQ0       1
#define pinQ1       2
#define pinQ2       4
#define pinQ3       8
#define pinQ4      16
#define pinQ5      32
#define pinQ6      64
#define pinQ7     128
int SR1 = 0, SR2 = 0, SR3 = 255, SR4 = 255, SR5 = 255, SR6 = 255;

// Shift Lights
#define sRPM1    6800
#define sRPM2    7400
#define sRPM3    8000
#define sRPM4    8600
#define sRPM5    9200
#define sRPM6    9800
#define sRPM7   10400
#define sRPM8   11000
#define sRPM9   11600
#define sRPM10  12000
#define sRPM11  12200
#define sRPM12  12300
#define sLED1   pinQ2 //SR2 
#define sLED2   pinQ3 //SR2 
#define sLED3   pinQ4 //SR2 
#define sLED4   pinQ5 //SR2 
#define sLED5   pinQ6 //SR2
#define sLED6   pinQ7 //SR2
#define sLED7   pinQ7 //SR1
#define sLED8   pinQ6 //SR1
#define sLED9   pinQ2 //SR1
#define sLED10  pinQ3 //SR1
#define sLED11  pinQ4 //SR1
#define sLED12  pinQ5 //SR1
#define rpmFlashTime 75 // RPM Flash timing (ms)

// Warning Lights
#define ledLR  pinQ4
#define ledLG  pinQ3
#define ledLB  pinQ2
#define ledRR  pinQ7
#define ledRG  pinQ6
#define ledRB  pinQ5
#define engTempLow        60    //Blue Blink
#define engTempMedLow     70    //Blue
#define engTempMedHigh    95    //Yellow
#define engTempHigh       105   //Red
#define engTempFlashTime  500   //Time in ms
#define batVoltMed        13    //Yellow
#define batVoltLow        12.5  //Red
int launchArm = 0;

// 7 segment
#define ssC pinQ1 //SR3
#define ssD pinQ2 //SR3  
#define ssE pinQ3 //SR3
#define ssG pinQ4 //SR3
#define ssF pinQ5 //SR3
#define ssA pinQ6 //SR3
#define ssB pinQ7 //SR3
int ssCount = 1;
int seg1 = (ssB + ssC); // 1
int seg2 = (ssA + ssB + ssD + ssE + ssG); // 2
int seg3 = (ssA + ssB + ssC + ssD + ssG); // 3
int seg4 = (ssB + ssC + ssF + ssG); // 4
int seg5 = (ssA + ssC + ssD + ssF + ssG); // 5
int seg6 = (ssA + ssC + ssD + ssE + ssF + ssG); // 6

// CAN Bus 
const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

/* Current CANBus Setup on ECU
PE1: RPM, TPS, Fuel time/angle 
PE2: Barometer, MAP, Lambda
PE6: Bat Volt, Temps
*/

#define PE1  0xF048
#define PE2  0xF148
#define PE3  0xF248
#define PE4  0xF348
#define PE5  0xF448
#define PE6  0xF548
#define PE7  0xF648
#define PE8  0xF748
#define PE9  0xF848
#define PE10 0xF948
#define PE11 0xFA48
#define PE12 0xFB48
#define PE13 0xFC48
#define PE14 0xFD48
#define PE15 0xFE48
#define PE16 0xFF48
#define PDMs 0X0500
#define PDM1 0x7FF    // FAN | GLOBAL ERROR | BAT VOLT
unsigned char len = 0;
unsigned char buf[64]; // This was 8 in other dash code but doesnt matter
unsigned int canID;

// CAN Value Variables 
// RPM
boolean updateRPM = false;
int rpm = 0;
int rpmLast = 0;
// Wheel Speed
float avgWheelSpeed = 0;
// Engine Temp
float engTemp = 0;
float engTempLast = 0;
int updateEngTemp = 0;
// Battery Voltage
float batVolt = 0;
float batVoltLast = 0;
int updateBatVolt = 0;
// Oil Pressure --
float oilPres = 0;
float oilPresLast = 0;
float oilPresLow = 23;
float oilPresHigh = 90;
long oilPresFlashLast = 0;
byte oilPresFlash = 1;
int updateOilPres = 0;
// TPS -----------
int tps = 0;
int tpsLast = 0;
int updateTPS = 0;
// Fan -----------
boolean fan = false;
// PDM -----------
boolean pdmError = false;
boolean pdmFaulted = false;

// OLED 
#define OLED_MOSI  9        //Data
#define OLED_CLK   8        //Clk
#define OLED_DC    5//11    //DC/SA0
#define OLED_CS    6//13    //CS
#define OLED_RESET A1       //Rst
Adafruit_SSD1306 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
int OLEDMode = 1;
int allowCycle = 0;
int switchMode = 0;
// Formula Logo --
 static const unsigned char PROGMEM QFSAELogo[] = {
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000001, B11111111, B11100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000111, B11111111, B10000000, B00000000,
   B00000000, B00000000, B11111111, B11111000, B00000000, B00000000, B00000000, B00000011, B00000000, B00000000, B00000000, B00000000, B00011111, B11111111, B00000000, B00000000,
   B00000000, B00000000, B01100000, B00011100, B00000000, B00000000, B00000000, B00011111, B11100000, B00000000, B00000000, B00000000, B00111000, B00000110, B00000000, B00000000,
   B00000000, B00000000, B00110000, B00000111, B11111111, B11111111, B11111111, B11111100, B11111111, B11111111, B11111111, B11111111, B11100000, B00001100, B00000000, B00000000,
   B00000000, B00000000, B00011000, B00000011, B11111111, B11111111, B11111111, B11111000, B00111111, B11111111, B11111111, B11111111, B11000000, B00011000, B00000000, B00000000,
   B00000000, B00000000, B00001100, B00000000, B00000000, B00000000, B00000000, B01100000, B00011100, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00000000,
   B00000000, B00000000, B00000110, B00000000, B00000000, B00000000, B00000001, B11000000, B00001110, B00000000, B00000000, B00000000, B00000000, B01100000, B00000000, B00000000,
   B00000000, B00000000, B00000011, B11111111, B11111111, B10000000, B00000001, B10000000, B00000111, B00000000, B00000011, B11111111, B11111111, B11000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00001111, B11111111, B11111111, B11111111, B10000000, B00000011, B11111111, B11111111, B11111111, B11110000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000011, B00000000, B00000011, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000110, B00000000, B00000001, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001110, B00000000, B00000000, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00011100, B00000000, B00000000, B11100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00011100, B00000000, B00000000, B01100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00011111, B11111111, B11111111, B11000000, B00000000, B00000000, B00111000, B00000000, B00000000, B01110000, B00000000, B00000000, B00000111, B11111111, B11111111, B11100000,
   B01111111, B11111111, B11111111, B11111111, B00000000, B00000000, B01110000, B00000000, B00000000, B00111000, B00000000, B00000001, B11111111, B11111111, B11111111, B11111000,
   B11111100, B00000000, B00000011, B11111111, B11000000, B00000000, B11100000, B00000000, B00000000, B00011100, B00000000, B00001111, B11111111, B00000000, B00000000, B11111100,
   B11110000, B00000000, B00000000, B00000011, B11100000, B00000000, B11000000, B00000111, B11000000, B00001110, B00000000, B00001111, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11100000, B00000000, B11000000, B00011111, B11110000, B00001110, B00000000, B00011110, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11100000, B00000001, B11000000, B00111100, B01111000, B00000110, B00000000, B00011100, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B01100000, B00000011, B00000111, B11110000, B00011111, B10000011, B00000000, B00011000, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B01100111, B11111111, B11111111, B11100000, B00001111, B11111111, B11111111, B10011000, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11111111, B11111111, B00001111, B11000000, B00000111, B11000001, B11111111, B11111100, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11100000, B00000000, B11111011, B10000000, B00000011, B11111110, B00000000, B00011100, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11100000, B00001111, B10000111, B00000000, B00000001, B11000111, B11100000, B00011100, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11100000, B11111000, B00001110, B00000000, B00000000, B11100000, B00111100, B00011100, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11100111, B10000000, B00011100, B00000000, B00000000, B01110000, B00000111, B10011100, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B11101100, B00000000, B00011100, B00000000, B00000000, B01110000, B00000000, B11011110, B00000000, B00000000, B00000000, B00111100,
   B11110000, B00000000, B00000000, B00000000, B01100000, B00000000, B00111000, B00000000, B00000000, B00111000, B00000000, B00011000, B00000000, B00000000, B00000000, B00111100,
   B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00111000,
   B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00111000,
   B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00111000,
   B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00111000,
   B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00111000,
   B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00111000,
   B01110000, B00011111, B11111110, B00111111, B11111110, B01111111, B11110001, B11000000, B00001100, B11000000, B00001011, B00000000, B00000000, B00010000, B00000000, B00111000,
   B01111000, B00011111, B11111111, B01111111, B11111111, B01111111, B11111101, B11100000, B00011101, B11000000, B00011011, B00000100, B00000000, B00111000, B00000000, B01111000,
   B01111111, B00001111, B11111111, B01110000, B00000111, B01111111, B11111101, B11110000, B00111101, B11000000, B00011011, B00000110, B00000000, B00011100, B00000111, B11111000,
   B00111111, B00000000, B00000000, B01100000, B00000011, B00000000, B00011101, B11111000, B01111101, B11000000, B00011011, B00011111, B00000000, B00001110, B00000111, B11110000,
   B00000000, B00011111, B11111111, B01100000, B00000011, B01111111, B11111001, B11011111, B11101101, B11000000, B00011011, B00001110, B00000000, B00000111, B00000000, B00000000,
   B00000000, B00011111, B11111111, B01100000, B00000011, B01111111, B11111101, B11001111, B11001101, B11000000, B00011011, B00000100, B00000001, B11111111, B10000000, B00000000,
   B00000000, B00011000, B00000000, B01110000, B00000111, B01100000, B00001101, B11000111, B10001101, B11000000, B00011011, B00000000, B00000011, B11111111, B11000000, B00000000,
   B00000000, B00011000, B00000000, B01111111, B11111111, B01100000, B00001101, B11000000, B00001101, B11111111, B11111011, B11111111, B11100111, B00000000, B11100000, B00000000,
   B00000000, B00011000, B00000000, B00111111, B11111110, B01100000, B00001101, B11000000, B00001100, B11111111, B11111011, B11111111, B11101110, B00000000, B01110000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, B00000000, B00100000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00011111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
   B00000000, B00000000, B00000000
 };

// Inputs 
#define cyclePin     A0
#define pitComPIN     A1
#define launchArmPIN  A2
int pitCom = 0;
bool startUp = false;

// Top level initialization
void setup() {
  Serial.begin(115200);
  Serial.println("--------- Upper Dash V2 ---------");
  initShiftRegisters(); // Initialize Shift Registers
  initDisplay(); // Initializes display and puts logo on screen
  initSequence1(); // Light show
  initCAN(); // Initialize CAN
  initSequence2();
  delay(100);
//  if(waitForStartup()){
//    startUpSequence();
//  }
}

// Main sequence
void loop() {
  resetSR(); // Reset shift register values
  updateData(); // read data from CAN and adjust variables
  shiftLights(); // update SR1, SR2
  statusLights(); // update SR4, SR5, SR6
  //dashCycle();
  dispTPS();
  updateDash(); // Update the lights on the dash
}

// Initialize Shift Registers
void initShiftRegisters(){
  pinMode(latchPIN, OUTPUT);
  pinMode(clockPIN, OUTPUT);
  pinMode(dataPIN, OUTPUT);
  pinMode(cyclePin, INPUT);
  digitalWrite(latchPIN, LOW);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
  digitalWrite(latchPIN, HIGH);
}

void initDisplay(){
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
  dispLogo();
}

// Initialize CANBus connection
void initCAN() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("CAN Init");
  display.display();
  // We could do something sick here for while its connecting to CAN
  while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 250k
  {
    Serial.println("Connecting to CAN");
  }
  display.setCursor(0,0);
  display.println("SUCESS!");
  display.display();
  //Serial.println("CAN BUS Shield init ok!");
  // Print something to the dash to say initializing complete
}

/* more cool stuff */
void initSequence1() {
  // light each shift light one at a time up and down
  for (int i = 1; i <= 24; i++) {
    switch (i) {
      case 1: SR2 += sLED1; break;
      case 2: SR2 += sLED2; break;
      case 3: SR2 += sLED3; break;
      case 4: SR2 += sLED4; break;
      case 5: SR2 += sLED5; break;
      case 6: SR2 += sLED6; break;
      case 7: SR1 += sLED7; break;
      case 8: SR1 += sLED8; break;
      case 9: SR1 += sLED9; break;
      case 10: SR1 += sLED10; break;
      case 11: SR1 += sLED11; break;
      case 12: SR1 += sLED12; break;
      case 13: SR1 -= sLED12; break;
      case 14: SR1 -= sLED11; break;
      case 15: SR1 -= sLED10; break;
      case 16: SR1 -= sLED9; break;
      case 17: SR1 -= sLED8; break;
      case 18: SR1 -= sLED7; break;
      case 19: SR2 -= sLED6; break;
      case 20: SR2 -= sLED5; break;
      case 21: SR2 -= sLED4; break;
      case 22: SR2 -= sLED3; break;
      case 23: SR2 -= sLED2; break;
      case 24: SR2 -= sLED1; break;
    }
    //randomize colors on the Status LEDS
    int colours[] = {111, 183, 219}; // red green blue
    randomSeed(analogRead(0));
    SR4 = colours[random(0, 3)];
    SR5 = colours[random(0, 3)];
    SR6 = colours[random(0, 3)];

    digitalWrite(latchPIN, LOW);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
    digitalWrite(latchPIN, HIGH);

    delay(25);
  }
}

// After CAN init
void initSequence2(){
  for(int j = 0; j < 3; j++){
    for (int i = 1; i <= 15; i++) {
      switch (i) {
        case 1: SR2 += sLED1; break;
        case 2: SR2 += sLED2; break;
        case 3: SR2 += sLED3; break;
        case 4: SR2 += sLED4; SR2 -= sLED1; break;
        case 5: SR2 += sLED5; SR2 -= sLED2; break;
        case 6: SR2 += sLED6; SR2 -= sLED3; break;
        case 7: SR1 += sLED7; SR2 -= sLED4; break;
        case 8: SR1 += sLED8; SR2 -= sLED5; break;
        case 9: SR1 += sLED9; SR2 -= sLED6; break;
        case 10: SR1 += sLED10; SR2 -= sLED7; break;
        case 11: SR1 += sLED11; SR2 -= sLED8; break;
        case 12: SR1 += sLED12; SR2 -= sLED9; break;
        case 13: SR1 -= sLED10; break;
        case 14: SR1 -= sLED11; break;
        case 15: SR1 -= sLED12; break;
      }
      //randomize colors on the Status LEDS
      int colours[] = {111, 183, 219}; // red green blue
      randomSeed(analogRead(0));
      SR4 = colours[random(0, 3)];
      SR5 = colours[random(0, 3)];
      SR6 = colours[random(0, 3)];

      digitalWrite(latchPIN, LOW);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
      shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
      digitalWrite(latchPIN, HIGH);
      delay(150);
    }
  }
}

/* waiting for car to start
Dash has full functionality except shift lights */
bool waitForStartup(){
  const int startTime = millis();
  while(!startUp){
    resetSR(); // Reset shift register values
    updateData(); // read data from CAN and adjust variables
    statusLights(); // update SR4, SR5, SR6
    dashCycle();
    updateDash(); // Update the lights on the dash
    if(rpm > 1000 && (millis() - startTime) > 1000){ // Car has been started for one sec
      return true; // Will exit this loop when car starts
    }
  }
}

// Cool dash sequence when car is started
void startUpSequence() {
  // light each shift light one at a time up and down
  for (int i = 1; i <= 24; i++) {
    switch (i) {
      case 1: SR2 += sLED1; break;
      case 2: SR2 += sLED2; break;
      case 3: SR2 += sLED3; break;
      case 4: SR2 += sLED4; break;
      case 5: SR2 += sLED5; break;
      case 6: SR2 += sLED6; break;
      case 7: SR1 += sLED7; break;
      case 8: SR1 += sLED8; break;
      case 9: SR1 += sLED9; break;
      case 10: SR1 += sLED10; break;
      case 11: SR1 += sLED11; break;
      case 12: SR1 += sLED12; break;
      case 13: SR1 -= sLED12; delay(100); break;
      case 14: SR1 -= sLED11; delay(100); break;
      case 15: SR1 -= sLED10; delay(100); break;
      case 16: SR1 -= sLED9; delay(100); break;
      case 17: SR1 -= sLED8; delay(100); break;
      case 18: SR1 -= sLED7; delay(100); break;
      case 19: SR2 -= sLED6; delay(100); break;
      case 20: SR2 -= sLED5; delay(100); break;
      case 21: SR2 -= sLED4; delay(100); break;
      case 22: SR2 -= sLED3; delay(100); break;
      case 23: SR2 -= sLED2; delay(100); break;
      case 24: SR2 -= sLED1; delay(100); break;
    }
    //randomize colors on the Status LEDS
    int colours[] = {111, 183, 219}; // red green blue
    if(i < 8){
      SR4 = colours[0];
      SR5 = colours[1];
      SR6 = colours[2];
    }
    else if (i < 16){
      SR4 = colours[1];
      SR5 = colours[2];
      SR6 = colours[0];
    }
    else {
      SR4 = colours[2];
      SR5 = colours[0];
      SR6 = colours[1];
    }

    digitalWrite(latchPIN, LOW);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
    shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
    digitalWrite(latchPIN, HIGH);
    delay(50);
  }
}

// Reset shift register values
void resetSR(){
  SR1 = 0;
  SR2 = 0;
  SR3 = 255;
  SR4 = 255;
  SR5 = 255;
  SR6 = 255;
}

// Update data from CAN
void updateData() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {          // check if data coming
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    canID = CAN.getCanId();
    /* For debugging */
    if (canID == PE1) { // PE1 
      // RPM 
      rpm = buf[1] * 256 + buf[0];
      Serial.println(rpm);
      if (rpm != rpmLast)
        updateRPM = true;
      rpmLast = rpm;
      // TPS 
      tps = buf[3] * 256 + buf[2];
      if (tps > 32767)
        tps -= 65536;
      tps *= 0.1;
      if (tps != tpsLast)
        updateTPS = 1;
      tpsLast = tps;
      Serial.println("TPS: ");
      Serial.print(tps);
    } else if (canID == PE3) { // PE3 
      // Oil Pressure 
      oilPres = buf[7] * 256 + buf[6];
      if (oilPres > 32767)
        oilPres -= 65536;
      oilPres *= 0.001;
      if (oilPres != oilPresLast)
        updateOilPres = 1;
      oilPresLast = oilPres;
    } else if (canID == PE6) { // PE6 
      // Engine Temp 
      engTemp = buf[5] * 256 + buf[4];
      if (engTemp > 32767)
        engTemp -= 65536;
      engTemp *= 0.1;
      if (engTemp != engTempLast)
        updateEngTemp = 1;
      engTempLast = engTemp;
    } else if (canID == PE5) { // PE5 
      // Average Wheel Speed 
      avgWheelSpeed = buf[7] * 256 + buf[6];
      if (avgWheelSpeed > 32767)
        avgWheelSpeed -= 65536;
      avgWheelSpeed *= 0.2;
    } else if (canID == PDM1) { // PDM1 
      // Fan 
      fan = buf[0];
      // Battery Voltage
      batVolt = buf[2] * 0.1216;
      if (batVolt != batVoltLast)
        updateBatVolt = 1;
      batVoltLast = batVolt;
      // PDM Global Error
      if (buf[1] == 1) {
        pdmFaulted = true;
      }
      pdmError = buf[1];
    }
  }
}

// Update shift lights
void shiftLights() {
  // Should we add blinking when 12 lights are on??
  if (rpm > sRPM12) {
    // 12 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
  }
  else if (rpm > sRPM11) {
    // 11 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8 + sLED9 + sLED10 + sLED11;
  }
  else if (rpm > sRPM10) {
    // 10 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8 + sLED9 + sLED10;
  }
  else if (rpm > sRPM9) {
    // 9 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8 + sLED9;
  }
  else if (rpm > sRPM8) {
    // 8 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7 + sLED8;
  }
  else if (rpm > sRPM7) {
    // 7 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
    SR1 += sLED7;
  }
  else if (rpm > sRPM6) {
    // 6 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5 + sLED6;
  }
  else if (rpm > sRPM5) {
    // 5 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4 + sLED5;
  }
  else if (rpm > sRPM4) {
    // 4 lights
    SR2 += sLED1 + sLED2 + sLED3 + sLED4;
  }
  else if (rpm > sRPM3) {
    // 3 lights
    SR2 += sLED1 + sLED2 + sLED3;
  }
  else if (rpm > sRPM2) {
    // 2 lights
    SR2 += sLED1 + sLED2;
  }
  else if (rpm > sRPM1) {
    // 1 lights
    SR2 += sLED1;
  }
}

// Update status lights
void statusLights() {
  // TMP    FAN
  // BAT    PDM
  // OIL    ARM
  // TMP --
  if (engTemp < engTempLow) {
    if (millis() - engTempFlashTime > engTempFlashLast) {
      engTempFlashLast = millis();
      engTempFlash = engTempFlash * -1;
    }
    if (engTempFlash == 1) {
      SR6 -= ledLB;
    }
  } else if (engTemp < engTempMedLow) {
    // (blue) getting there..
    SR6 -= ledLB;
  } else if (engTemp > engTempHigh) {
    // (red) too hot!
    SR6 -= ledLR;
  } else if (engTemp > engTempMedHigh) {
    // (yellow) pretty hot..
    SR6 -= ledLR + ledLG;
  } else {
    // (green) good to go!
    SR6 -= ledLG;
  }
  // FAN --
  if (fan) {
    // purple if fan is on
    SR6 -= ledRR + ledRB;
  }
  // BAT --
  if (batVolt < batVoltLow) {
    //red, if voltage is less than 12.5v
    SR5 -= ledLR;
  } else if (batVolt < batVoltMed) {
    //yellow, if voltage is less than 13v
    SR5 -= ledLR + ledLG;
  } else {
    //otherwise green light
    SR5 -= ledLG;
  }
  // PDM ----
  if (pdmError) {
    // red light if pdm error
    SR5 -= ledRR;
  } else if (pdmFaulted) {
    // yellow light if pdm fault
    SR5 -= ledRR + ledRG;
  } else {
    // green light if good
    SR5 -= ledRG;
  }
  // OIL ----
  if (oilPres < oilPresLow) {
    // blinking red light if oil pressure is less than 23
    if (millis() - engTempFlashTime > oilPresFlashLast) {
      oilPresFlashLast = millis();
      oilPresFlash = oilPresFlash * -1;
    }
    if (oilPresFlash == 1) {
      SR4 -= ledLR;
    }
  } else if (oilPres > oilPresHigh) {
    // red light if oil pressure is greater than 90
    SR4 -= ledLR;
  } else {
    SR4 -= ledLG;
  }
  // ARM ----
  if (launchArm) {
    SR4 -= ledRG + ledRR;
  }
}

// Push changes to dash
void updateDash(){
  digitalWrite(latchPIN, LOW);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR6);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR5);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR4);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR3);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR2);
  shiftOut(dataPIN, clockPIN, MSBFIRST, SR1);
  digitalWrite(latchPIN, HIGH);
}

// Cycle dash modes
void dashCycle() {
  if (digitalRead(cyclePin) == 0) {
    currentCycle%=5;
    currentCycle++;
    delay(500);
  }
  switch (currentCycle) {
    case 1: dispBatVolt(); break;
    case 2: dispOilPres(); break;
    case 3: dispEngTemp(); break;
    case 4: dispTPS(); break;
    case 5: dispLogo(); break;
    default: SR3 -= seg1; break;
  }
}

// Display battery voltage
void dispBatVolt(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Bat Volt");
  display.setTextSize(5);
  display.setCursor(0,25);
  display.println(batVolt,1);
  display.display();
}

// Display oil pressure
void dispOilPres(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Oil Pres:");
  display.setTextSize(5);
  display.setCursor(0,25);
  display.println(oilPres,1);
  display.display();
}

// Display eng temp
void dispEngTemp(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Engine Temp:");
  display.setTextSize(5);
  display.setCursor(0,25);
  display.println(engTemp,1);
  display.display();
}

// Display TPS
void dispTPS(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("TPS");
  display.setTextSize(5);
  display.setCursor(0,25);
  display.println(tps);
  display.display();
}

// Display logo
void dispLogo() {
  //Serial.println("Displaying logo");
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 7, QFSAELogo, 128, 50, WHITE);
  display.display();
}
