/*
   Tim Sinaeve (c) 2017

   A testsketch for the FastLED library.
   (https://github.com/FastLED/FastLED)

   Testroutines were copied from examples provided with the library.
 */
#include "Arduino.h"

#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <PrintEx.h>

#define DATA_PIN    11
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120

/*
  LCD character display 16x2, 20x4, etc.
  Interface:  I2C
  Type:       LCD
  Resolution: 16 x 2 characters
*/

LiquidCrystal_I2C Lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
PrintEx lcd = Lcd;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

bool gReverseDirection = false;

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for(int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm(uint8_t BeatsPerMinute)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
   //CRGBPalette16 palette = PartyColors_p;
  CRGBPalette16 palette = RainbowStripeColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

//The setup function is called once at startup of the sketch
void setup()
{
  Lcd.begin(16, 2);
  Lcd.backlight();
  Lcd.clear();
  Lcd.setCursor(0, 0);
  lcd.printf("H=%3dS=%3dV=%3d ");
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

int i, j, k, l;

// The loop function is called in an endless loop
void loop()
{
  // read potentiometers
  i = analogRead(A0) >> 2;
  j = map(analogRead(A1), 219 , 1023, 0, 255);
  k = map(analogRead(A2), 0, 915, 0, 255);
  l = analogRead(A3) >> 7;
  Lcd.setCursor(0, 0);
  lcd.printf("H%-4d S%-4d V%-4d ", i, j, k);

  if (l == 0)
  {
    Lcd.setCursor(0, 1);
    lcd.print("confetti        ");
    confetti();
  }
  else if (l == 1)
  {
    Lcd.setCursor(0, 1);
    lcd.print("juggle          ");
    juggle();
  }
  else if (l == 2)
  {
    Lcd.setCursor(0, 1);
    lcd.print("sinelon         ");
    sinelon();
  }
  else if (l == 3)
  {
    Lcd.setCursor(0, 1);
    lcd.print("bpm             ");
    bpm(analogRead(A6) >> 2);
  }
  else if (l == 4)
  {
    Lcd.setCursor(0, 1);
    lcd.print("fire            ");
    Fire2012();
  }
  else
  {
    Lcd.setCursor(0, 1);
    lcd.print("set HSV values  ");
    for (int s = 0; s < 8; s++)
    {
      leds[s].setHSV(i, j, k);
    }
  }
  FastLED.show();
}
