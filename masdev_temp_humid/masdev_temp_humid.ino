// -------------------------------------------------------------------------------
// Project Masdev is distributed under the MIT License
// Copyright (c) 2012 Dan Malec
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -------------------------------------------------------------------------------

// -------------------------------------------------------------------------------
// Dependencies
// -------------------------------------------------------------------------------
// Adafruit Industries' real time clock library: https://github.com/adafruit/RTClib
// Adafruit Industries' LCD library: https://github.com/adafruit/LiquidCrystal
// Adafruit Industries' DHT sensor library: https://github.com/adafruit/DHT-sensor-library
// -------------------------------------------------------------------------------
#include "DHT.h"
#include "LiquidCrystal.h"
#include "RTClib.h"
#include "Wire.h"

// -------------------------------------------------------------------------------
// Config Constants
// -------------------------------------------------------------------------------
#define LCD_I2C_ADDR 0
#define LCD_COLUMNS 16
#define LCD_ROWS 2

#define DATE_ROW 0
#define DATE_COL 0

#define TIME_ROW 0
#define TIME_COL 8

#define DHT_SENSOR_PIN 2

#define HUMID_ROW 1
#define HUMID_COL 0

#define TEMP_ROW 1
#define TEMP_COL 8

// -------------------------------------------------------------------------------
// Modules
// -------------------------------------------------------------------------------
DHT dht(DHT_SENSOR_PIN, DHT22);
LiquidCrystal lcd(LCD_I2C_ADDR);
RTC_DS1307 rtc;

//! Setup and initialize.
void setup() {
  Wire.begin();

  // initialize the DHT
  dht.begin();

  // initialize the LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.setBacklight(HIGH);
  lcd.clear();

  // initialize the RTC
  rtc.begin();
  if (!rtc.isrunning()) {
    // if this is the first time talking to the RTC,
    // set the time to the sketch compile time
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

//! Print a zero padded number to the LCD.
/*!
  Prints a number to the LCD, left padding to two digits using a zero if
  needed.

  \param value the value to print to the LCD
*/
void print_padded_digit(int value) {
  if (value < 10) {
    lcd.print('0');
  }
  lcd.print(value, DEC);
}

//! Print the month, day, and time to the LCD.
/*!
  Prints the date in mm/dd format and the time in HH:MM:SS to the LCD.
  
  \param now the current date and time
*/
void print_date_time(DateTime *now) {
  lcd.setCursor(DATE_COL, DATE_ROW);
  print_padded_digit(now->month());
  lcd.print('/');
  print_padded_digit(now->day());

  lcd.setCursor(TIME_COL, TIME_ROW);
  print_padded_digit(now->hour());
  lcd.print(':');
  print_padded_digit(now->minute());
  lcd.print(':');
  print_padded_digit(now->second());
}

//! Print the temperature to the LCD.
/*!
  Prints the temperature to the LCD.
  
  \param temperature the temperature
*/
void print_temperature(float temperature) {
  lcd.setCursor(TEMP_COL, TEMP_ROW);
  print_padded_digit((int)(temperature * 1.8 + 32.0));
  lcd.print('F');
}

//! Print the humidity to the LCD.
/*!
  Prints the humidity to the LCD.
  
  \param humidity the humidity
*/
void print_humidity(float humidity) {
  lcd.setCursor(HUMID_COL, HUMID_ROW);
  print_padded_digit((int)humidity);
  lcd.print('%');
}

//! Main execution loop.
void loop() {
  DateTime now = rtc.now();

  print_date_time(&now);
  print_humidity(dht.readHumidity());
  print_temperature(dht.readTemperature());
  
  delay(100);
}

