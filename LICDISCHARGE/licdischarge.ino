#include <OneWire.h> 
#include "DallasTemperature.h"
#include <SPI.h>
#include <SD.h>
#include <DS1307.h>
#include <avr/sleep.h>
#define ONE_WIRE_BUS 2 

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

int ledpin=13, count=0;
const int  chipSelect=53;
double T=0, V1=0,V2=0, vsum1=0,vsum2=0;
File dataFile;
// Init the DS1307
DS1307 rtc(SDA, SCL);

void iamalive()
{
  digitalWrite(ledpin, HIGH);
  delay(100);
  digitalWrite(ledpin, LOW);
  delay(100); 
}
void delay60sec()
{
  for(int i=0; i<60; i++)
  {
    delay(100);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(ledpin, OUTPUT);
  Serial.begin(9600);
  sensors.begin();

  // Initialize the rtc object
  rtc.begin();
  // Set the clock to run-mode
  rtc.halt(false);
  
  // The following lines can be uncommented to set the time
  //rtc.setDOW(THURSDAY);        // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 10, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(06, 04, 2017);   // Set the date to October 3th, 2010
   if (!SD.begin(chipSelect)) {
    Serial.println("card intialized");
    return;
   }
}


void loop() {
 sensors.requestTemperatures(); 
 T = sensors.getTempCByIndex(0);  //Tenperature
 
   for(int i=0; i<20; i++)
   {
      vsum1 += analogRead(A0);
      vsum2 += analogRead(A1);
      delay(50);
   }
   vsum1/=20.0;  //actual reading
   vsum2/=20.0;  //actual reading
   V1 = (vsum1*5.0)/1024.0; //actual voltage --- to be calibrated
   V2 = (vsum2*5.0)/1024.0; //actual voltage --- to be calibrated

   String datetime = String(rtc.getDateStr()) + " " + String(rtc.getTimeStr());
   Serial.println(datetime + " ," + "T="+String(T)+ ", VA0="+String(V1)+ ", VA1="+String(V2));
   dataFile = SD.open("data.txt", FILE_WRITE);
     if(dataFile) 
       {
          dataFile.println(String(++count) +","+ datetime + ", T="+String(T)+ ", VA0="+String(V1)+ ", VA1="+String(V2));
          iamalive();
       }
       dataFile.close(); 
       delay60sec();
}




