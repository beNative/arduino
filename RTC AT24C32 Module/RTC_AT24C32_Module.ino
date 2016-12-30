/*
  DS3231 AT24C32 RTC module demo

  Read all about these modules on:

  http://tronixlabs.com.au/news/tutorial-using-ds1307-and-ds3231-realtime-clock-modules-with-arduino/

  Along with keeping track of the time and date, these modules also have a small
  EEPROM, an alarm function (DS3231 only) and the ability to generate a
  square-wave of various frequencies.
*/

#include <Wire.h>
// note: we use our custom library
#include <LiquidCrystal_I2C.h>

#define DS3231_I2C_ADDRESS 0x68

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void initDisplay(LiquidCrystal_I2C &lcd)
{
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  initDisplay(lcd);

  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
   //setDS3231time(0,13,19,5,30,12, 16);
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek,
  byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  lcd.setCursor(4,0);
  char s[16];
  sprintf(s, "%02d:%02d:%02d", hour, minute, second);
  lcd.print(s);
  lcd.setCursor(3, 1);
  sprintf(s, "%02d/%02d/20%2d", dayOfMonth, month, year);
  lcd.print(s);
}

void loop()
{
  displayTime();
  delay(500);
}
