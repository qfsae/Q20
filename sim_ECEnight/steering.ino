/* Sample code for Steering Wheel Electronics with data from CAN from assetto 

- Need to add 14-seg to this
- Also add potentiometer for neopixels
*/

#include <Adafruit_NeoPixel.h>
#include "mcp_can.h"
#include <SPI.h>

#define SPI_CS_PIN 10

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);   // Init NeoPixel Strip
MCP_CAN CAN(SPI_CS_PIN);

unsigned long rpmID = 0x00;
unsigned long tpsID = 0x08;
unsigned long gearID = 0xC0;
byte gear;
int rpm;
float tps;

void setup() {
  Serial.begin(115200);
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    delay(100);
  }
//   Serial.println("CAN Bus Initialized!");
}


void loop() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf);
        unsigned long id = CAN.getCanId();
        if(DEBUG){
            Serial.print("Getting Data from ID: ");
            Serial.println(id, HEX);

            for (int i = 0; i < len; i++)
            {
                Serial.print(buf[i]);
                Serial.print("\t");
            }
        }
        // Store variables from CAN - don't think that this is right atm
        if (id == rpmID){
            rpm = buf[0];
        } 
        if (id == tpsID){
            tps = buf[0];
        }
        if (id == gearID){
            gear = buf[0];
        }
    }
    mapShiftLights();
    mapGear();
}

void mapShiftLights(){
    if(rpm < 1000 && rpm > 500)  strip.setPixelColor(0, 0, 255, 0);      // last interval show all lights red
    if(rpm < 1500) strip.setPixelColor(1, 0, 255, 0);
    if(rpm < 2000) strip.setPixelColor(2, 0, 255, 0);
    if(rpm < 2500) strip.setPixelColor(3, 0, 255, 0);
    if(rpm < 3000) strip.setPixelColor(4, 0, 255, 0);
    if(rpm < 3500) strip.setPixelColor(5, 0, 0, 255);
    if(rpm < 4000) strip.setPixelColor(6, 0, 0, 255);
    if(rpm < 4500) strip.setPixelColor(7, 0, 0, 255);
    if(rpm < 5000) strip.setPixelColor(8, 0, 0, 255);
    if(rpm < 5500) strip.setPixelColor(9, 0, 0, 255);
    if(rpm < 6000) strip.setPixelColor(10, 255, 0, 0);
    if(rpm < 6500) strip.setPixelColor(11, 255, 0, 0);
    if(rpm < 7000) strip.setPixelColor(12, 255, 0, 0);
    if(rpm < 7500) strip.setPixelColor(13, 255, 0, 0);
    if(rpm < 8000) strip.setPixelColor(14, 255, 0, 0);
    if(rpm < 8500) batchPixels(0, 15, 255, 0, 0);
    strip.show();
}

void mapGear(){
    // Add code to map to the 14-segment display
}

