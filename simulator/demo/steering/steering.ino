#include "mcp_can.h"
#include <Adafruit_NeoPixel.h>


#define brightnessPin A0
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Use pin 10 for CAN shield version 1.2
#define SPI_CS_PIN 10
#define TOGGLE 2
int rpm;
int gear;
int tps;
volatile int pattern = 0;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  strip.begin();
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(1);
  }
  pinMode(TOGGLE, INPUT);
//  attachInterrupt(digitalPinToInterrupt(TOGGLE), cycle, RISING);
  Serial.println("Initialization Success");
}

void loop() {
  mapPattern();
  strip.clear();
  updateCAN();
  mapShiftLights();
//  mapGear();
}

int LED_COUNT = NUMPIXELS;

void mapShiftLights()
{
    int brightMax = 255;
    int brightData = analogRead(brightnessPin);
    int brightness = map(brightData, 0, 1023, 0, brightMax);
    //int brightness = 255;
    int maxRPM = 6000;
    int RPMi = map(rpm, 0, maxRPM, 0, NUMPIXELS - 1);

    switch (pattern)
    {
    case 0:        
        // Normal shift light pattern that we already have working
        for (int i = 0; i < RPMi; i++)
        {
            if (i < LED_COUNT/3) // First third
            {
                strip.setPixelColor(i, 0, brightness, 0); // Green
            }
            else if (i < (2*LED_COUNT)/3) // Second third
            {
                strip.setPixelColor(i, 0, 0, brightness); // Red
            }
            else // Last third
            {
                strip.setPixelColor(i, brightness, 0, 0); // Blue
            }
        }
        break;
     case 1: // Want this to progress inwards
      strip.clear();
          if (rpm > 1250){
            strip.setPixelColor(0, brightness, 0, 0);//red
            strip.setPixelColor(15, brightness, 0, 0);//red
          }
         if (rpm > 1600){
            strip.setPixelColor(1, brightness, 0, 0);//red
            strip.setPixelColor(14, brightness, 0, 0);//red
          }
         if (rpm > 2000){
          strip.setPixelColor(2, brightness, 0, 0);//red
          strip.setPixelColor(13, brightness, 0, 0);//red
         }
         if (rpm > 2500){
          strip.setPixelColor(3, 0, brightness, 0);//green
          strip.setPixelColor(12, 0, brightness, 0);//green
         }
          if (rpm > 3500){
          strip.setPixelColor(4, 0, brightness, 0);//green
          strip.setPixelColor(11, 0, brightness, 0);//green
         }
         if (rpm > 4000){
          strip.setPixelColor(5, 0, brightness, 0);//green
          strip.setPixelColor(10, 0, brightness, 0);//green
         }
         if (rpm > 4500){
          strip.setPixelColor(6, 0, 0, brightness);//blue
          strip.setPixelColor(9, 0, 0, brightness);//blue
         }
         if (rpm > 5000){
          strip.setPixelColor(7, 0, 0, brightness);//blue
          strip.setPixelColor(8, 0, 0, brightness);//blue
         }

       
      //  break;
    default: if (rpm > 6000){
        for(int i = 0; i < NUMPIXELS; i++){
          strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 5700){
        for(int i = 0; i < NUMPIXELS - 1; i++){
          strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 5400){
        for(int i = 0; i < NUMPIXELS - 2; i++){
          strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 5100){
          for(int i = 0; i < NUMPIXELS - 3; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 4800){
          for(int i = 0; i < NUMPIXELS - 4; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 4500){
          for(int i = 0; i < NUMPIXELS - 5; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
         if (rpm > 4200){
          for(int i = 0; i < NUMPIXELS - 6; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
         if (rpm > 3900){
          for(int i = 0; i < NUMPIXELS - 7; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
         if (rpm > 3600){
          for(int i = 0; i < NUMPIXELS - 8; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
         if (rpm > 3300){
          for(int i = 0; i < NUMPIXELS - 9; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 3000){
          for(int i = 0; i < NUMPIXELS -  10; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 2600){
          for(int i = 0; i < NUMPIXELS -  11; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 2100){
          for(int i = 0; i < NUMPIXELS -  12; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 1800){
          for(int i = 0; i < NUMPIXELS -  13; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 1500){
          for(int i = 0; i < NUMPIXELS -  14; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
        if (rpm > 1250){
          for(int i = 0; i < NUMPIXELS -  15; i++){
            strip.setPixelColor(i, 255, 255, 255);
          }
        }
       
        break;
    }
    strip.show();
}
void updateCAN(){
  unsigned char len = 0;

  // messages have max length of 8 bytes
  unsigned char buf[8];

  // Check if there is a message available
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    unsigned long id = CAN.getCanId();
//    Serial.print("Getting Data from ID: ");
//    Serial.println(id, HEX);
//
//    for (int i = 0; i < len; i++) {
//      Serial.print(buf[i]);
//      Serial.print("\t");
//    }
    // Brightness of LED depends on potentiometer position on the sending CAN
    // node
  rpm = buf[0] + 256*buf[1];
  gear = buf[3];
//  Serial.print(rpm);
//  Serial.print("\t");
//  Serial.print(gear);
//  Serial.println();
  }
}

void cycle(){
  Serial.print("Cycling Pattern");
  //pattern = pattern++ % 2;
}

void mapPattern(){
  int pot2Volts = analogRead(A1);
  if (pot2Volts < 500){
    //use case 0
    pattern = 0;
  }
  else {
    pattern = 1;
  }
}
