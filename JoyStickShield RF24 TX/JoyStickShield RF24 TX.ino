/* 
  JoyStickShield RF24TX

  Transmits Joystick values over a couple of NRF40 transceiver modules

  +---------------------+   +---------------------+   +---------------------+
  |  NRF24 Transceiver  |   |  Joystick Shield    |   |  LCD Display (I2C)  |
  +----------+----------+   +----------+----------+   +----------+----------+
  | NRF24L01 | Arduino  |   | Shield   | Arduino  |   | Display  | Arduino  |
  +----------+----------+   +----------+----------+   +----------+----------+
  | VCC      | 3.3 V    |   | VCC      | 5.0 V    |   | VCC      | 5.0 V    |
  | GND      | GND      |   | GND      | GND      |   | GND      | GND      |
  | CE       | D09      |   | UP       | D02      |   | SDA      | A4-SDA   |
  | CSN      | D10-SS   |   | RIGHT    | D03      |   | SCL      | A5-SCL   |
  | MOSI     | D11-MOSI |   | DOWN     | D04      |   +----------+----------+
  | MISO     | D12-MISO |   | LEFT     | D05      |
  | SCK      | D13-SCK  |   | E        | D06      |
  | IRQ      | NC       |   | F        | D07      |
  +----------+----------+   | SELECT   | D08      |
                            | X-POT    | A0       |
                            | Y-POT    | A1       |
                            +----------+----------+
  
Based on examples at http://www.bajdi.com/

Code altered by Tim Sinaeve to support the Funduino JoyStick Shield V1.0A.

REMARK: The NRF module is quite power hungry and might not function correctly
        if it is powered through the USB-port of the Arduino.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const byte PIN_BUTTON_UP     = 2;
const byte PIN_BUTTON_RIGHT  = 3;
const byte PIN_BUTTON_DOWN   = 4;
const byte PIN_BUTTON_LEFT   = 5;
const byte PIN_BUTTON_E      = 6; // Additional buttons on the Funduino JoyStick Shield V1.0A
const byte PIN_BUTTON_F      = 7;
const byte PIN_BUTTON_SELECT = 8; // Select button is triggered when joystick is pressed

const byte JOYSTICK_X = A0;
const byte JOYSTICK_Y = A1;

// NRF pins
const byte CE_PIN     = 9;
const byte CSN_PIN    = 10;

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int joystick[9];  // 9 element array holding Joystick readings
char X[8] = "";
char Y[8] = "";

void setup()
{
	// Specify each pin connected to a pushbutton as an input.
	// Also enable the Arduino's internal "pull-up" resistors
	// for each pushbutton we want to read--this means the shield
	// doesn't need to have resistors on it.
	// Note that when a pull-up resistor is used on a pin the
	// meaning of the values read are reversed compared to their
	// usual meanings:
	//    * HIGH = the button is not pressed
	//    * LOW = the button is pressed
	pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
	pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
	pinMode(PIN_BUTTON_SELECT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_E, INPUT_PULLUP);
	pinMode(PIN_BUTTON_F, INPUT_PULLUP);
	
	radio.begin();
	radio.openWritingPipe(pipe);

/*
	lcd.begin(16, 2); // initialize LCD
	lcd.backlight();  // switch backlight on
	lcd.setCursor(0, 0);
*/
}

void loop()
{
	joystick[0] = analogRead(JOYSTICK_X);
	joystick[1] = analogRead(JOYSTICK_Y);
	joystick[2] = digitalRead(PIN_BUTTON_SELECT);
	joystick[3] = digitalRead(PIN_BUTTON_UP);
	joystick[4] = digitalRead(PIN_BUTTON_DOWN);
	joystick[5] = digitalRead(PIN_BUTTON_LEFT);
	joystick[6] = digitalRead(PIN_BUTTON_RIGHT);
	joystick[7] = digitalRead(PIN_BUTTON_E);
	joystick[8] = digitalRead(PIN_BUTTON_F);
	
	/*
	sprintf(X, "X=%3d S=%1dU=%1dD=%1d", joystick[0], joystick[2], joystick[3], joystick[4]);
	lcd.setCursor(0, 0);
	lcd.print(X);
	lcd.setCursor(0, 1);
	sprintf(Y, "Y=%3d ", joystick[1]);
	lcd.print(Y);
	*/

	radio.write(joystick, sizeof(joystick));
}
