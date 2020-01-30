/* Sample code for Steering Wheel Electronics with data from CAN from assetto
- Need to add 14-seg to this
- Also add potentiometer for neopixels
*/
#include <Adafruit_NeoPixel.h>
#include "mcp_can.h"
#include <SPI.h>

#define SPI_CS_PIN 10

/* 14-Segment Definitions */
#define A 0 // top
#define B 1 // top right
#define C 3 // bottom right
#define D 4 // bottom
#define E 5 // bottom left
#define F 6 // top left
#define G 7 // middle
#define H 8 // top diagonal
#define I 9 // bottom diagonal

/* Neopixels */
#define LED_PIN 10
#define LED_COUNT 16
#define LED_PIN 6
#define brightnessPin A0

#define patternToggle 2 // interrupt pin
int pattern = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Init NeoPixel Strip
MCP_CAN CAN(SPI_CS_PIN);                                           // Init CAN

unsigned long rpmID = 0x00;
unsigned long tpsID = 0x08;
unsigned long gearID = 0xC0;
byte gear;
int rpm;
float tps;

void setup()
{
    Serial.begin(115200);
    pinMode(brightnessPin, INPUT);
    pinMode(LED_PIN, OUTPUT);

    pinMode(patternToggle, INPUT);
    attachInterrupt(digitalPinToInterrupt(patternToggle, lightCycle, LOW));

    while (CAN.begin(CAN_500KBPS) != CAN_OK)
    {
        delay(100);
    }

    lightPattern = 0;
    //   Serial.println("CAN Bus Initialized!");
}

void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];
    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf);
        unsigned long id = CAN.getCanId();
        if (0)
        {
            Serial.print("Getting Data from ID: ");
            Serial.println(id, HEX);
            for (int i = 0; i < len; i++)
            {
                Serial.print(buf[i]);
                Serial.print("\t");
            }
        }
        // Store variables from CAN - don't think that this is right atm
        if (id == rpmID)
        {
            rpm = buf[0];
        }
        if (id == tpsID)
        {
            tps = buf[0];
        }
        if (id == gearID)
        {
            gear = buf[0];
        }
    }
    mapShiftLights();
    mapGear();
}

void mapShiftLights()
{
    int brightMax = 100;
    int brightData = analogRead(brightnessPin);
    int brightness = map(brightData, 0, 1023, 0, brightMax);
    int maxRPM = 7500;
    int RPMi = map(rpm, 0, maxRPM, 0, LED_COUNT - 1);

    switch()
    for (int i = 0; i < RPMi; i++)
    {
        if (i < (LED_COUNT / 3))
        {
            strip.setPixelColor(i, 0, brightness, 0);
        }
        else if (i < (2 * LED_COUNT / 3))
        {
            strip.setPixelColor(i, 0, 0, brightness);
        }
        else
        {
            strip.setPixelColor(i, brightness, 0, 0);
        }
    }
    strip.show();

    if (rpm < 1000 && rpm > 500)
        strip.setPixelColor(0, 0, 255, 0); // last interval show all lights red
    if (rpm < 1500)
        strip.setPixelColor(1, 0, 255, 0);
    if (rpm < 2000)
        strip.setPixelColor(2, 0, 255, 0);
    if (rpm < 2500)
        strip.setPixelColor(3, 0, 255, 0);
    if (rpm < 3000)
        strip.setPixelColor(4, 0, 255, 0);
    if (rpm < 3500)
        strip.setPixelColor(5, 0, 0, 255);
    if (rpm < 4000)
        strip.setPixelColor(6, 0, 0, 255);
    if (rpm < 4500)
        strip.setPixelColor(7, 0, 0, 255);
    if (rpm < 5000)
        strip.setPixelColor(8, 0, 0, 255);
    if (rpm < 5500)
        strip.setPixelColor(9, 0, 0, 255);
    if (rpm < 6000)
        strip.setPixelColor(10, 255, 0, 0);
    if (rpm < 6500)
        strip.setPixelColor(11, 255, 0, 0);
    if (rpm < 7000)
        strip.setPixelColor(12, 255, 0, 0);
    if (rpm < 7500)
        strip.setPixelColor(13, 255, 0, 0);
    if (rpm < 8000)
        strip.setPixelColor(14, 255, 0, 0);
    if (rpm < 8500)
        batchPixels(0, 15, 255, 0, 0);
    strip.show();
}

void mapGear(){
    // Reset Gears
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(H, HIGH);
    digitalWrite(I, HIGH);

    switch(gear){
        case -1: // reverse
            digitalWrite(A, HIGH);
            digitalWrite(B, HIGH);
            digitalWrite(E, HIGH);
            digitalWrite(F, HIGH);
            digitalWrite(G, HIGH);
            digitalWrite(I, HIGH);
            break;
        case 0:
            digitalWrite(B, HIGH);
            digitalWrite(C, HIGH);
            digitalWrite(E, HIGH);
            digitalWrite(F, HIGH);
            digitalWrite(H, HIGH);
            digitalWrite(I, HIGH);
            break;
        case 1:
            digitalWrite(B, HIGH);
            digitalWrite(C, HIGH);
            break;
        case 2:
            digitalWrite(A, HIGH);
            digitalWrite(B, HIGH);
            digitalWrite(G, HIGH);
            digitalWrite(E, HIGH);
            digitalWrite(D, HIGH);
            break;
        default:
            // write nothing
            break;
    }
    // Add code to map to the 14-segment display
}
