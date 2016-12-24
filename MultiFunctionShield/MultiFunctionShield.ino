#include <Arduino.h>
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

/*
 Setup:
   A0 : On board potentiometer
   A1 : BUTTON_1_PIN: Beeps buzzer
   A2 : BUTTON_2_PIN: switches relais on D5 ON/OFF
   A3 : BUTTON_3_PIN:
   A4 : LM35_PIN
   A5 : Potentiometer.

   D13: LED_1_PIN (D1)
   D12: LED_2_PIN (D2)
   D11: LED_3_PIN (D3)
   D10: LED_4_PIN (D4)

   D5 : Relais
 */

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
//  Serial.begin(9600);
  pinMode(DD5, OUTPUT);
  MFS.write("LE0S");
  delay(300);
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
  delay(1000);

  MFS.write("8eep");
  delay(300);
  MFS.beep();
  delay(300);


}

int counter = 0;
char c;
int i;
byte b;
boolean r = 0;
int n = 0;
int k = 0;
char *s;

void loop() {
  i = analogRead(A5);
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
}
