#include <Arduino.h>
#include <FastLED.h>

using namespace std;

// DEFINE PINS HERE

/*
                       GND |         | Vin
                         0 |         | Analog GND
                         1 |         | 3.3V
                LED-01   2 |         | 23 (A9)  LED-11
                         3 |         | 22 (A8)  LED-10
                         4 |         | 21 (A7)  LED-07
                LED-08   5 |         | 20 (A6)  LED-06
                LED-05   6 |         | 19 (A5)  SCL0
                LED-03   7 |         | 18 (A4)  SDA0
                LED-04   8 |         | 17 (A3)
                LED-12   9 |         | 16 (A2)
                LED-13  10 |         | 15 (A1)  LED-09
                LED-15  11 |         | 14 (A0)  LED-02
                LED-16  12 |         | 13 (led) LED-14
                      3.3v |         | GND
                        24 |         |    (A22)
                        25 |         |    (A22)
                        26 |         | 39 (A20)
                        27 |         | 38 (A19)  AUDIO_INPUT_PIN
                        28 |         | 37 (A18)  MIC_GAIN
                        29 |         | 36 (A17)
                        30 |         | 35 (A16)
                  (A12) 31 |         | 34 (A15)
     BATTERY_PIN  (A13) 32 |         | 33 (A14)
*/


// STATE
uint_fast32_t loops = 0;
uint_fast32_t setupTime = 0;
uint_fast32_t loggingTimestamp = 0;
uint_fast8_t hue = 0;

// GEOMETRY CONSTANTS
const uint_fast8_t rows = 100;
const uint_fast8_t columns = 4;
const uint_fast16_t numLEDs = rows * columns;
CRGB leds[numLEDs];

// FUNTION DEFINITIONS
void clear();
void setAll(CRGB color);

void setup() {
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");

  // SETUP LEDS
  // Parallel  Pin layouts on the teensy 3/3.1:
  // // WS2811_PORTD: 2,14,7,8,6,20,21,5
  // // WS2811_PORTC: 15,22,23,9,10,13,11,12,28,27,29,30 (these last 4 are pads on the bottom of the teensy)
  // WS2811_PORTDC: 2,14,7,8,6,20,21,5,15,22,23,9,10,13,11,12 - 16 way parallel

  FastLED.addLeds<WS2811_PORTD,columns>(leds, rows);
  FastLED.setDither(0);

  // FastLED.setBrightness(255);

  // INDICATE BOOT SEQUENCE
  setAll(0x001000);
  FastLED.show();
  delay(3000);

  // DISPLAY STUFF
  clear();
  FastLED.show();

  randomSeed(analogRead(A4));

  Serial.println("setup complete");

  setupTime = millis();
}

// LOOP
void loop() {
  loops++;
  uint_fast32_t currentTime = millis();

  if (currentTime > loggingTimestamp + 5000) {
    loggingTimestamp = currentTime;

    Serial.print(currentTime);

    Serial.print("\tFrame Rate: ");
    Serial.print(loops / ((currentTime - setupTime) / 1000));
    Serial.println("");
  }

  clear();

  hue = (currentTime / 50) % 255;

  CRGB color = CHSV(hue, 240, 255);

  // for (uint_fast16_t i=0; i<numLEDs; i++) {
  //   if ((loops + i) % 6 == 0)
  //   leds[i] = color;
  // }

  // for (uint_fast16_t i=0; i<numLEDs; i++) {

  //   if (random8(6) < 3)
  //   leds[i] = color;
  // }

  uint_fast8_t r = random8();
  for (uint_fast16_t i=0; i<numLEDs; i++) {
    if ((r + i) % 4 == 0)
    leds[i] = color;
  }

  FastLED.show();

  // delayMicroseconds(21);
}



void setAll(CRGB color) {
  for (uint_fast16_t i=0; i<numLEDs; i++) {
    leds[i] = color;
  }
}

void clear() {
  setAll(0x000000);
}
