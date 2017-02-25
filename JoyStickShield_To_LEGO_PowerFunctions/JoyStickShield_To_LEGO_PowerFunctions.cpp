/*
  Author: Tim Sinaeve
  (c) 2017

  Can operate Lego Power functions motors and servo's with proportional control.

  3 IR-LED's in series connected to output 8

  This sketch uses an NRF24 transceiver and 3 IR-lets to receive signals from a
  NRF24 transmitter and translate them into IR-signals that are compatible with
  LEGO's powerfunctions.
*/

#include <PowerFunctions.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// these channels correspond to the selected position of the switch on the
// LEGO power functions receiver.
const int PF_CHANNEL1 = 0;
const int PF_CHANNEL2 = 1;
const int PF_CHANNEL3 = 2;
const int PF_CHANNEL4 = 3;

// Arduino pin connected to the IR-leds
const byte PIN_IR_LED_CHANNEL1 = 8;

PowerFunctions pf1(PIN_IR_LED_CHANNEL1, PF_CHANNEL1);

struct JoyStick {
  bool up;
  bool down;
  bool left;
  bool right;
  bool select;
  bool e;
  bool f;
  int x;
  int y;
};

JoyStick joystick;

const byte CE_PIN  = 9;
const byte CSN_PIN = 10;

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe (LongLong)

RF24 radio(CE_PIN, CSN_PIN); // NRF radio object

void setup() {
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void loop() {

  while (radio.available())
  {
    // Read the data payload until we've received everything
      radio.read(&joystick, sizeof(joystick));
      int x = joystick.x;
      int y = joystick.y;
      int s; // steering
      if ((y > 505) && (y < 514))
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
      pf1.combo_pwm(s, t);
  }
}

