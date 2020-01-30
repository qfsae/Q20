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

int rpm;
int gear;
int tps;
int pattern = 0;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  strip.begin();
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(1);
  }
  Serial.println("Initialization Success");
}

void loop() {
  strip.clear();
  updateCAN();
  mapShiftLights();
}

int LED_COUNT = NUMPIXELS;

void mapShiftLights()
{
    int brightMax = 255;
    int brightData = analogRead(brightnessPin);
//    int brightness = map(brightData, 0, 1023, 0, brightMax);
    int brightness = 255;
    int maxRPM = 7000;
    int RPMi = map(rpm, 0, maxRPM, 0, NUMPIXELS - 1);

    switch (pattern)
    {
    case 0: // Normal shift light pattern that we already have working
        for (int i = 0; i < RPMi; i++)
        {
            if (i < LED_COUNT/3) // First third
            {
                strip.setPixelColor(i, 0, brightness, 0); // Green
            }
            else if (i < (2*LED_COUNT)/3) // Second third
            {
                strip.setPixelColor(i, brightness, 0, 0); // Red
            }
            else // Last third
            {
                strip.setPixelColor(i, 0, 0, brightness); // Blue
            }
        }
        break;
    case 1: // Want this to progress inwards
        break;
    default:
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
  Serial.print(rpm);
  Serial.print("\t");
  Serial.print(gear);
  Serial.println();
  }
}
