#include "Arduino.h"
#include <Wire.h>
#include <Encoder.h>
#include <Switch.h> // switch class
// note: we use our custom library
#include <LiquidCrystal_I2C.h>

/*
  Change these pin numbers to the pins connected to your encoder.
     Best Performance: both pins have interrupt capability
     Good Performance: only the first pin has interrupt capability
     Low Performance:  neither pin has interrupt capability

     tip:  avoid using pins with LEDs attached
*/

/*
  I2C display support
*/

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C lcd(0x27, 16, 2);
Switch b1 = Switch(4);
Switch b2 = Switch(7);
Switch b3 = Switch(10);

void initDisplay(LiquidCrystal_I2C &lcd)
{
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enc1  Enc2  Enc3");
}

Encoder enc1(3, 2);
Encoder enc2(6, 5);
Encoder enc3(9, 8);

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Test:");
  initDisplay(lcd);
}

long enc1Pos = -999;
long enc2Pos = -999;
long enc3Pos = -999;

void loop() {
  long newEnc1Pos, newEnc2Pos, newEnc3Pos;
  newEnc1Pos = enc1.read();
  newEnc2Pos = enc2.read();
  newEnc3Pos = enc3.read();
  b1.poll();
  b2.poll();
  b3.poll();

  if (b1.switched()) {
    enc1.write(0);
  }
  if (b2.switched()) {
    enc2.write(0);
  }
  if (b3.switched()) {
    enc3.write(0);
  }
  if (newEnc1Pos != enc1Pos) {
    Serial.print("Enc1 = ");
    Serial.print(newEnc1Pos);
    lcd.setCursor(0, 1);
    lcd.print("    ");
    lcd.setCursor(0, 1);
    lcd.print(newEnc1Pos);

    Serial.println();
    enc1Pos = newEnc1Pos;
  }
  if (newEnc2Pos != enc2Pos) {
    Serial.print("Enc2 = ");
    Serial.print(newEnc2Pos);
    lcd.setCursor(6, 1);
    lcd.print("    ");
    lcd.setCursor(6, 1);
    lcd.print(newEnc2Pos);

    Serial.println();
    enc2Pos = newEnc2Pos;
  }
  if (newEnc3Pos != enc3Pos) {
    Serial.print("Enc3 = ");
    Serial.print(newEnc3Pos);
    lcd.setCursor(12, 1);
    lcd.print("    ");
    lcd.setCursor(12, 1);
    lcd.print(newEnc3Pos);

    Serial.println();
    enc3Pos = newEnc3Pos;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    //knobLeft.write(0);
  }
}
