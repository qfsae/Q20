#include <Adafruit_NeoPixel.h>

#define DIN 11
#define PWR 2
#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, DIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(PWR, OUTPUT);
  pinMode(DIN, OUTPUT);
  digitalWrite(PWR, HIGH);
  strip.begin();
}

void loop() {
  for (int i = 1; i < 100; i++) {
    for (int k = 0; k < LED_COUNT; k++) {
      strip.clear();
      strip.setPixelColor(k, i, 0, i);
      strip.show();
      delay(100);
    }
  }
}
