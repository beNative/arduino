/*
  Author: Tim Sinaeve
  (c) 2015

  Uses Joystick shield V1.A.

  Can operate Lego Power functions motors and servo's with proportional control.
*/

#include <PowerFunctions.h>

PowerFunctions pf(12, 0); // IR led connected to Arduino pin 12, channel 1

// Arduino pin setup
// analog pins
const byte PIN_ANALOG_X      = 0; 
const byte PIN_ANALOG_Y      = 1;

// digital pins
const byte PIN_BUTTON_SELECT = 1; // gets triggered when joystick is pressed
const byte PIN_BUTTON_UP     = 2;
const byte PIN_BUTTON_RIGHT  = 3;
const byte PIN_BUTTON_DOWN   = 4;
const byte PIN_BUTTON_LEFT   = 5;

void setup() {
  // Specify each pin connected to a pushbutton as an input.
  // Also enable the Arduino's internal "pull-up" resistors
  // for each pushbutton we want to read--this means the shield
  // doesn't need to have resistors on it.
  // Note that when a pull-up resistor is used on a pin the
  // meaning of the values read are reversed compared to their
  // usual meanings:
  //    * HIGH = the button is not pressed
  //    * LOW = the button is pressed
  pinMode(PIN_BUTTON_RIGHT, INPUT);
  digitalWrite(PIN_BUTTON_RIGHT, HIGH);

  pinMode(PIN_BUTTON_LEFT, INPUT);
  digitalWrite(PIN_BUTTON_LEFT, HIGH);

  pinMode(PIN_BUTTON_UP, INPUT);
  digitalWrite(PIN_BUTTON_UP, HIGH);

  pinMode(PIN_BUTTON_DOWN, INPUT);
  digitalWrite(PIN_BUTTON_DOWN, HIGH);

  pinMode(PIN_BUTTON_SELECT, INPUT);
  digitalWrite(PIN_BUTTON_SELECT, HIGH);
}

void loop() {
  int x = analogRead(PIN_ANALOG_X);
  int y = analogRead(PIN_ANALOG_Y);

  int l = digitalRead(PIN_BUTTON_LEFT);
  int r = digitalRead(PIN_BUTTON_RIGHT);
  int u = digitalRead(PIN_BUTTON_UP);
  int d = digitalRead(PIN_BUTTON_DOWN);

  int s; // steering
  if ((y > 507) && (y < 513))
  {
    s = 0;
  }
  else if (y > 513)
  {
    s = map(y, 513, 1023, 1, 7);
  }
  else
  {
    s = map(y, 0, 507, 9, 16);
  }

  int t; // throttle
  if (x >= 508)
  {
    t = map(x, 500, 1023, 0, 7);
  }
  else
  {
    t = map(x, 0, 508, 9, 16);
  }
  // send combined signal for both RED and BLUE channel
  pf.combo_pwm(s, t);
}
