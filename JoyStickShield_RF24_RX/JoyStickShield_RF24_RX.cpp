/*
Author: Tim Sinaeve

Funduino JoyStickShield v1.A RF24 RX

- Receives data from another transceiver with 2 analog values from a joystick.
- Displays received values on Serial Monitor

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

REMARK: The NRF module is quite power hungry and might not function correctly
        if it is powered through the USB-port of the Arduino.
*/

#include <SPI.h>
#include <nRF24L01.h> // uses https://github.com/TMRh20/RF24
#include <RF24.h>

const byte CE_PIN  = 9;
const byte CSN_PIN = 10;

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe (LongLong)

RF24 radio(CE_PIN, CSN_PIN); // NRF radio object

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

void setup() 
{
  Serial.begin(9600);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void loop() 
{
  while (radio.available())
  {
	 radio.read(&joystick, sizeof(joystick));
	 Serial.print("X = ");
	 Serial.print(joystick.x);
	 Serial.print(" Y = ");
	 Serial.print(joystick.y);
	 Serial.print(" S = ");
	 Serial.print(joystick.select);
	 Serial.print(" U = ");
	 Serial.print(joystick.up);
	 Serial.print(" D = ");
	 Serial.print(joystick.down);
	 Serial.print(" L = ");
	 Serial.print(joystick.left);
	 Serial.print(" R = ");
	 Serial.print(joystick.right);
	 Serial.print(" E = ");
	 Serial.print(joystick.e);
	 Serial.print(" F = ");
	 Serial.println(joystick.f);
  }
  Serial.println("No radio available");
}
