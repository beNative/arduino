#include <Arduino.h>
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

/*
  +-----+-------+------------------------+
  | Pin | Label | Description            |
  +-----+-------+------------------------+
  | A0  |       | Potentiometer          |
  | A1  | S1    | BUTTON_1_PIN           |
  | A2  | S2    | BUTTON_2_PIN           |
  | A3  | S3    | BUTTON_3_PIN           |
  | A4  |       |                        |
  | A5  | A5    | Analog header pin      |
  | D13 | D1    | LED_1_PIN              |
  | D12 | D2    | LED_2_PIN              |
  | D11 | D3    | LED_3_PIN              |
  | D10 | D4    | LED_4_PIN              |
  | D05 | ~5    | Digital PWM header pin |
  | D06 | ~6    | Digital PWM header pin |
  | D09 | ~9    | Digital PWM header pin |
  +-----+-------+------------------------+
 Setup:
   A1 : BUTTON_1_PIN: Beeps buzzer
   A2 : BUTTON_2_PIN: D5 output ON/OFF
   A3 : BUTTON_3_PIN: switches modes
   A4 : LM35_PIN
   A5 : Potentiometer or external sensor

   D5 : Relais
   D6 : PWM output controlled by A0 or A5
   D9 :
 */

void showLeds()
{
  // LED pins are active low so OFF means ON
  digitalWrite(LED_1_PIN, OFF);
  delay(100);
  digitalWrite(LED_2_PIN, OFF);
  delay(100);
  digitalWrite(LED_3_PIN, OFF);
  delay(100);
  digitalWrite(LED_4_PIN, OFF);
  delay(100);
  digitalWrite(LED_1_PIN, ON);
  delay(100);
  digitalWrite(LED_2_PIN, ON);
  delay(100);
  digitalWrite(LED_3_PIN, ON);
  delay(100);
  digitalWrite(LED_4_PIN, ON);
}

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
//  Serial.begin(9600);
  pinMode(DD5, OUTPUT);
  MFS.write("High");
  delay(300);
  showLeds();

  //setPwmFrequency(DD6, 1);  // 62500 Hz
  //setPwmFrequency(DD6, 1024);  // 62500 Hz

  pinMode(DD6, OUTPUT);
  /*
  MFS.write("beep");
  delay(300);
  MFS.beep();
  delay(300);
  */


}

int counter = 0;
char c;
int i;
byte b;
boolean r = 0;
int n = 0;
int k = 0;
char *s;
int mode = 1;

void loop() {
  if (mode == 0)
  {
    i = analogRead(A0);
  }
  else if (mode == 1)
  {
    i = analogRead(A4);
  }
  else if (mode == 2)
  {
    i = analogRead(A5);
  }

  analogWrite(DD6, i / 4);

  if (abs(map(i, 0, 1024, 0, 9999) - k) > 10)
  {
    k = map(i, 0, 1024, 0, 9999);
    MFS.write(k);
    Serial.println(k);
  }

  b = MFS.getButton();
  if (b == BUTTON_1_PRESSED)
  {
    MFS.write("8eep");
    MFS.beep();
    delay(300);
  }
  else if (b == BUTTON_2_PRESSED)
  {
    r = !r;
    if (r) {
      n = ON;
      s = "ON";
    }
    else
    {
      n = OFF;
      s = "OFF";
    }
    digitalWrite(DD5, n);
    MFS.write(s);
    delay(300);
  }
  else if (b == BUTTON_3_PRESSED)
  {
     mode += 1;
     mode %= 3;
     if (mode == 0)
       s = "A0";
     else if (mode == 1)
       s = "A4";
     else if (mode == 2)
       s = "A5";
     MFS.write(s);
     delay(300);

  }
}
