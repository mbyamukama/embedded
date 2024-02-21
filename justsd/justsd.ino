
#include <SPI.h>
#include <SD.h>

const int chipSelect = 53;
int i=0; //ow2
File dataFile;
String data="";

void setup()
{
  Serial.begin(38400);  // Start up serial
 // pinMode(10, OUTPUT);      //pb0 PIN 10 hardware chipselect must be an output for sd functions to work 
  if (!SD.begin(chipSelect)) {
    Serial.println("error");
    return;
  }
}

void loop()
{
             data = "testing" + String (++i);
             dataFile = SD.open("data.txt", FILE_WRITE);
                // if the file is available, write to it:
              if (dataFile) 
               {
                 dataFile.println(data);
                 data="";                 
               }
           dataFile.close();
           delay(100);
}
