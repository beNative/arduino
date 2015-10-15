/*
  Author: Tim Sinaeve (2015)
  
  Reads PWM signals from a RC receiver and transmits the signals to a IR diode
  to control Lego Power Functions in combination with the standard 2 channel
  LEGO IR receivers.
  The PWM signals are translated to Lego Powerfunctions signals with support for
  proportional control (7 steps per channel). 
  Use this to control your Lego Power Functions with the standard Lego IR
  receivers.

  This sketch is based on code from rcarduino.blogspot.com

  See related posts for the technical details about this setup:
    http://rcarduino.blogspot.co.uk/2012/01/how-to-read-rc-receiver-with.html
    http://rcarduino.blogspot.co.uk/2012/03/need-more-interrupts-to-read-more.html
    http://rcarduino.blogspot.co.uk/2012/01/can-i-control-more-than-x-servos-with.html
*/

#include <RCArduinoFastLib.h>
#include <PinChangeInt.h>
#include <PowerFunctions.h>

// Channel in pins
const byte THROTTLE_IN_PIN = 6;
const byte STEERING_IN_PIN = 5;
const byte AUX_IN_PIN      = 7;

// Channel out pins
const byte THROTTLE_OUT_PIN = 8;
const byte STEERING_OUT_PIN = 9;
const byte AUX_OUT_PIN      = 10;

// Servo indexes
const byte SERVO_THROTTLE    = 0;
const byte SERVO_STEERING    = 1;
const byte SERVO_AUX         = 2;
const byte SERVO_FRAME_SPACE = 3;

// These bit flags are set in bUpdateFlagsShared to indicate which
// channels have new signals
const byte THROTTLE_FLAG = 1;
const byte STEERING_FLAG = 2;
const byte AUX_FLAG      = 4;

// holds the update flags defined above
volatile uint8_t bUpdateFlagsShared;

/*
 Shared variables are updated by the ISR and read by loop.
 In the  loop we immediatley take local copies so that the ISR can keep ownership of the
 shared ones. To access these in loop we first turn interrupts off with noInterrupts,
 we take a copy to use in loop and the turn interrupts back on as quickly as possible.
 This ensures that we are always able to receive new signals.
*/
volatile uint16_t unThrottleInShared;
volatile uint16_t unSteeringInShared;
volatile uint16_t unAuxInShared;

/*
 These are used to record the rising edge of a pulse in the calcInput functions.
 They do not need to be volatile as they are only used in the ISR. If we wanted
 to refer to these in loop and the ISR then they would need to be declared as volatile.
*/
uint16_t unThrottleInStart;
uint16_t unSteeringInStart;
uint16_t unAuxInStart;

uint16_t unLastAuxIn           = 0;
uint32_t ulVariance            = 0;
uint32_t ulGetNextSampleMillis = 0;
uint16_t unMaxDifference       = 0;

PowerFunctions pf1(12, 0); // Arduino pin 12, channel 1
PowerFunctions pf2(12, 1); // Arduino pin 12, channel 2

// simple interrupt service routine
void calcThrottle()
{
  if (PCintPort::pinState)
  {
    unThrottleInStart = TCNT1;
  }
  else
  {
    unThrottleInShared = (TCNT1 - unThrottleInStart) >> 1;
    bUpdateFlagsShared |= THROTTLE_FLAG;
  }
}

void calcSteering()
{
  if (PCintPort::pinState)
  {
    unSteeringInStart = TCNT1;
  }
  else
  {
    unSteeringInShared = (TCNT1 - unSteeringInStart) >> 1;
    bUpdateFlagsShared |= STEERING_FLAG;
  }
}

void calcAux()
{
  if (PCintPort::pinState)
  {
    unAuxInStart = TCNT1;
  }
  else
  {
    unAuxInShared = (TCNT1 - unAuxInStart) >> 1;
    bUpdateFlagsShared |= AUX_FLAG;
  }
}

void setup()
{
  // sets a standard rate of 50 Hz by setting a frame space of
  // 10 * 2000 = 3 Servos + 7 times 2000
  CRCArduinoFastServos::setFrameSpaceA(SERVO_FRAME_SPACE, 7 * 2000);
  CRCArduinoFastServos::begin();

  // using the PinChangeInt library, attach the interrupts used to read the
  // channels
  PCintPort::attachInterrupt(THROTTLE_IN_PIN, calcThrottle, CHANGE);
  PCintPort::attachInterrupt(STEERING_IN_PIN, calcSteering, CHANGE);
  PCintPort::attachInterrupt(AUX_IN_PIN, calcAux, CHANGE);
}

void loop()
{
  // create local variables to hold a local copies of the channel inputs
  // these are declared static so that thier values will be retained
  // between calls to loop.
  static uint16_t unThrottleIn;
  static uint16_t unSteeringIn;
  static uint16_t unAuxIn;
  // local copy of update flags
  static uint8_t bUpdateFlags;
  // check shared update flags to see if any channels have a new signal
  if (bUpdateFlagsShared)
  {
    // turn interrupts off quickly while we take local copies of the shared
    // variables
    noInterrupts();
    // take a local copy of which channels were updated in case we need to
    // use this in the rest of loop
    bUpdateFlags = bUpdateFlagsShared;
    // in the current code, the shared values are always populated
    // so we could copy them without testing the flags
    // however in the future this could change, so lets
    // only copy when the flags tell us we can.
    if (bUpdateFlags & THROTTLE_FLAG)
    {
       unThrottleIn = unThrottleInShared;
    }
    if (bUpdateFlags & STEERING_FLAG)
    {
      unSteeringIn = unSteeringInShared;
    }
    if (bUpdateFlags & AUX_FLAG)
    {
      unAuxIn = unAuxInShared;
    }
    // clear shared copy of updated flags as we have already taken the updates
    // we still have a local copy if we need to use it in bUpdateFlags
    bUpdateFlagsShared = 0;
        // we have local copies of the inputs, so now we can turn interrupts
        // back on as soon as interrupts are back on, we can no longer use the
        // shared copies, the ISRs own these and could update them at any time.
        // During the update, the shared copies may contain junk. Luckily we
        // have our local copies to work with :-)
    interrupts();
  }
  bUpdateFlags = 0;
  int t;
  if (unThrottleIn < 1500)
  {
    t = map(unThrottleIn, 1100, 1500, 9, 16);
  }
  else if (unThrottleIn >= 1500)
  {
    t = map(unThrottleIn, 1500, 1900, 0, 7);
  }
  int s;
  if (unSteeringIn < 1500)
  {
    s = map(unSteeringIn, 1100, 1500, 9, 16);
  }
  else if (unSteeringIn >= 1500)
  {
    s = map(unSteeringIn, 1500, 1900, 0, 7);
  }
  int a;
  if (unAuxIn < 1500)
  {
    a = 0;
  }
  else
  {
    a = 1;
  }
  if ((s != 0) || (t != 0))
  {
    // send combined signal for both RED and BLUE channel
  }
  if (a == 1)
  {
    pf1.combo_pwm(t, s);
  }
  else
  {
    pf2.combo_pwm(t, s);
  }
}
