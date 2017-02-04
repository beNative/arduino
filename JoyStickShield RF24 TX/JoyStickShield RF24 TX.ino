/* 
  JoyStickShield RF24TX

  Transmits Joystick values over a couple of NRF24L01+ transceiver modules

  +---------------------+   +---------------------+   +---------------------+
  | NRF24 module (SPI)  |   |  Joystick Shield    |   |  LCD Display (I2C)  |
  +----------+----------+   +----------+----------+   +----------+----------+
  | NRF24L01 | Arduino  |   | Shield   | Arduino  |   | Display  | Arduino  |
  +----------+----------+   +----------+----------+   +----------+----------+
  | VCC      | 3.3 V    |   | VCC      | 5.0 V    |   | VCC      | 5.0 V    |
  | GND      | GND      |   | GND      | GND      |   | GND      | GND      |
  | CE       | D09      |   | UP    (A)| D02      |   | SDA      | A4-SDA   |
  | CSN      | D10-SS   |   | RIGHT (B)| D03      |   | SCL      | A5-SCL   |
  | MOSI     | D11-MOSI |   | DOWN  (C)| D04      |   +----------+----------+
  | MISO     | D12-MISO |   | LEFT  (D)| D05      |
  | SCK      | D13-SCK  |   | E        | D06      |
  | IRQ      | NC       |   | F        | D07      |
  +----------+----------+   | SELECT   | D08      |
                            | X-POT    | A0       |
                            | Y-POT    | A1       |
                            +----------+----------+
  
Code altered by Tim Sinaeve to support the Funduino JoyStick Shield V1.0A.

REMARK: The NRF module is quite power hungry and might not function correctly
        if it is powered through the USB-port of the Arduino.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const byte PIN_BUTTON_UP     = 2;
const byte PIN_BUTTON_RIGHT  = 3;
const byte PIN_BUTTON_DOWN   = 4;
const byte PIN_BUTTON_LEFT   = 5;
const byte PIN_BUTTON_E      = 6; // Additional buttons on the Funduino JoyStick Shield V1.0A
const byte PIN_BUTTON_F      = 7;
const byte PIN_BUTTON_SELECT = 8; // Select button is triggered when joystick is pressed
const byte JOYSTICK_X        = A0;
const byte JOYSTICK_Y = A1;

// NRF pins
const byte CE_PIN     = 9;
const byte CSN_PIN    = 10;

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

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

char X[9] = "        ";
char Y[9] = "        ";

//Custom Character #0 (arrow up)
byte SpecialChar0[8]={
 B00000,
 B00100,
 B01110,
 B10101,
 B00100,
 B00100,
 B00100,
 B00000
};

//Custom Character #1 (arrow down)
byte SpecialChar1[8]={
 B00000,
 B00100,
 B00100,
 B00100,
 B10101,
 B01110,
 B00100,
 B00000
};

void setup()
{
// Specify each pin connected to a pushbutton as an input.
// Also enable the Arduino's internal "pull-up" resistors
// for each pushbutton we want to read--this means the shield
// doesn't need to have resistors on it.
// Note that when a pull-up resistor is used on a pin the
// meaning of the values read are reversed compared to their
// usual meanings:
// * HIGH = the button is not pressed
// * LOW = the button is pressed
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SELECT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_E, INPUT_PULLUP);
  pinMode(PIN_BUTTON_F, INPUT_PULLUP);

  radio.begin();
  radio.openWritingPipe(pipe);

  lcd.begin(16, 2); // initialize LCD
  // create a new custom character (MUST be placed after call to begin()
  lcd.createChar(1, SpecialChar0); // we can create max 8 custom chars.
  lcd.createChar(2, SpecialChar1);
  lcd.backlight();  // switch backlight on
  lcd.setCursor(0, 0);
}

void loop()
{
  joystick.x = analogRead(JOYSTICK_X);
  joystick.y = analogRead(JOYSTICK_Y);

  joystick.select = digitalRead(PIN_BUTTON_SELECT);
  joystick.up     = digitalRead(PIN_BUTTON_UP);
  joystick.down   = digitalRead(PIN_BUTTON_DOWN);
  joystick.left   = digitalRead(PIN_BUTTON_LEFT);
  joystick.right  = digitalRead(PIN_BUTTON_RIGHT);
  joystick.e      = digitalRead(PIN_BUTTON_E);
  joystick.f      = digitalRead(PIN_BUTTON_F);

  radio.write(&joystick, sizeof(joystick));

  sprintf(
    X,
    "X=%4d|%s%s%s%s ",
    joystick.x,
    joystick.up?"  ":"\x01""A",
    joystick.down?"  ":"\x02""C",
    joystick.left?"  ":"\x7F""D",
    joystick.right?"  ":"\x7E""B"
  );
  lcd.setCursor(0, 0);
  lcd.print(X);
  lcd.setCursor(0, 1);
  sprintf(
    Y,
    "Y=%4d|%s %s%s",
    joystick.y,
    joystick.select?"      ":"SELECT",
    joystick.e?" ":"E",
    joystick.f?" ":"F"
  );
  lcd.print(Y);
}
