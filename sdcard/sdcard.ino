/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 Created 9 May 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */
#include <SPI.h>
#include <SD.h>

const int chipSelect = 3; //ow2
int j=0;
File dataFile;

void setup() {
    pinMode(10, OUTPUT);      //pb0 PIN 10 hardware chipselect must be an output
  if (!SD.begin(chipSelect)) {
    return;
  }
}


void loop() {
     dataFile = SD.open("data.txt", FILE_WRITE);
      // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("testing " + String(++j));
    dataFile.close();
    } 
    delay(2000);
  } 
