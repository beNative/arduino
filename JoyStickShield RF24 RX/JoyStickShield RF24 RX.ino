/*
Author: Tim Sinaeve

Funduino JoyStickShield v1.A RF24 RX

- Receives data from another transceiver with
  2 analog values from a joystick.
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

Based on examples at http://www.bajdi.com/

REMARK: The NRF module is quite power hungry and might not function correctly
        if it is powered through the USB-port of the Arduino.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <LiquidCrystal_I2C.h>

#define CE_PIN   9
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe (LongLong)

RF24 radio(CE_PIN, CSN_PIN); // NRF radio object

int joystick[9];  // 9 element array holding Joystick readings

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
    // Read the data payload until we've received everything
      radio.read(joystick, sizeof(joystick));
      Serial.print("X = ");
      Serial.print(joystick[0]);
      Serial.print(" Y = ");
      Serial.print(joystick[1]);
      Serial.print(" S = ");
      Serial.print(joystick[2]);
      Serial.print(" U = ");
      Serial.print(joystick[3]);
      digitalWrite(2, 1 - joystick[3]);
      Serial.print(" D = ");
      Serial.print(joystick[4]);
      digitalWrite(3, 1 - joystick[4]);
      Serial.print(" L = ");
      Serial.print(joystick[5]);
      digitalWrite(4, 1 - joystick[5]);
      Serial.print(" R = ");
      Serial.print(joystick[6]);
      digitalWrite(5, 1 - joystick[6]);
      Serial.print(" E = ");
      Serial.print(joystick[7]);
      digitalWrite(6, 1 - joystick[7]);
      digitalWrite(7, joystick[8]);
      Serial.print(" F = ");
      Serial.println(joystick[8]);
  }
  Serial.println("No radio available");
}
