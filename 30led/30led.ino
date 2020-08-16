#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 30
#define LED_TYPE WS2812B

CRGB leds[NUM_LEDS];

void setup() {
  LEDS.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.begin(9600);
}

void loop()
{
  patternOne();
}

void patternOne() {
  int red = random(256);
  int green = random(256);
  int blue =  random(256);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(red, green, blue);
    FastLED.show();
    delay(10);
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i].setRGB(red, green, blue);
    FastLED.show();
    delay(10);
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
  }
}

void patternTwo() {
  int red = random(256);
  int green = random(256);
  int blue =  random(256);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(red, green, blue);
    FastLED.show();
    delay(10);
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
  }
}
