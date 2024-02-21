/* Sleep Demo Serial
 * -----------------
 * Example code to demonstrate the sleep functions in a Arduino. Arduino will wake up
 * when new data is received in the serial port USART
 * Based on Sleep Demo Serial from http://www.arduino.cc/playground/Learning/ArduinoSleepCode 
 *
 * Copyright (C) 2006 MacSimski 2006-12-30 
 * Copyright (C) 2007 D. Cuartielles 2007-07-08 - Mexico DF
 * 
 *  With modifications from Ruben Laguna  2008-10-15
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <avr/power.h>
#include <avr/sleep.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define DS3231_I2C_ADDRESS 0x68

const int chipSelect = 4;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte seconds, minute, hour, dayOfWeek, dayOfMonth, month, year;



void setup() {
  // initialize serial:
  pinMode(9, OUTPUT);
  Serial.begin(38400);
  Wire.begin();
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
      if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        // don't do anything more:
       // return;
    }
}

void alive()
{
  digitalWrite(9, HIGH);
  delay(20);
  digitalWrite(9, LOW);
  delay(20);
}

void sleep()
{     
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here
  sleep_enable();          // enables the sleep bit in the mcucr register  so sleep is possible. just a safety pin 
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  
  sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         
  power_all_enable();   //enable all modules
}


void loop()
{
  sleep();
  // print the string when a newline arrives:
  if (stringComplete) {
// open the file. note that only one file can be open at a time, so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(getTimeString() + " : " + inputString);
    dataFile.close();
    alive();
  } 
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}



/**********************RTC FUNCTIONS************************************/

byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void readRTCTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
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

String getTimeString()
{
  // retrieve data from DS3231
 readRTCTime(&seconds, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 
 String time = "";
 if(dayOfMonth<10) { time = time + "0"+String(dayOfMonth);} else {time = String(dayOfMonth);}
 if(month<10) { time = time + '/' + "0"+String(month);} else {time = time + '/' +String(month);}
time = time + '/' +String(year);
time = time + " ";
if(hour<10) { time = time + "0"+String(hour);} else {time = time +String(hour);}
if(minute<10) { time = time + ':' + "0"+String(minute);} else {time = time + ':' +String(minute);}
if(seconds<10) { time = time + ':' + "0"+String(seconds);} else {time = time + ':' +String(seconds);}
  return time;
}



