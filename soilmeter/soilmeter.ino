#include <OneWire.h> 
#include "DallasTemperature.h"
#include <SPI.h>
#include <SD.h>
#include <DS1307.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define ONE_WIRE_BUS 2 

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

int ledpin=13, sleep_count=0, count=0;
const int  chipSelect=4;
double T=0, V1=0,vsum1=0;
File dataFile;
// Init the DS1307
DS1307 rtc(SDA, SCL);

void iamalive()
{
  digitalWrite(ledpin, 1);
  delay(100);
  digitalWrite(ledpin, 0);
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
  rtc.setDOW(THURSDAY);        // Set Day-of-Week to SUNDAY
  rtc.setTime(9, 52, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(05, 04, 2017);   // Set the date to October 3th, 2010
  if (!SD.begin(chipSelect)) {
    // return;
  }
  wdt_enable(WDTO_8S); //WDT to wake every 8 seconds
}


void loop() {
  //sleep now
  sleep();  //watchdog will wake up MCU and execution will start from here
  
  if(sleep_count>7)  //check if 56 seconds elapsed (wdt wakes up and increments sleep_count every 8s)
  {
    wdt_disable();      //disable watchdog
    sleep_count=0;  //reset counter

    /*Now measure*/
    sensors.requestTemperatures(); 
    T = sensors.getTempCByIndex(0);  //Tenperature
    for(int i=0; i<20; i++)
    {
      vsum1 += analogRead(A0);
      delay(50);
    }
    vsum1/=20.0;  //actual reading
    V1 = (vsum1*5.0)/1024.0; //actual voltage --- to be calibrated
    
    String datetime = String(rtc.getDateStr()) + " " + String(rtc.getTimeStr());
    Serial.println(String(++count) +","+ datetime + " ," + "T="+String(T)+ ", VA0="+String(V1));
    dataFile = SD.open("data.txt", FILE_WRITE);
    if(dataFile) 
    {
      dataFile.println(String(++count) +","+ datetime + " ," + "T="+String(T)+ ", VA0="+String(V1));
      iamalive();
    }
    dataFile.close(); 
    wdt_enable(WDTO_8S); //WDT to wake every 8 seconds
  }

}

void sleep()   
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode.
  sleep_enable(); // Enable sleep mode.
  sleep_mode(); // Enter sleep mode.
  // After waking from watchdog interrupt the code continues to execute from this point.
  sleep_disable(); // Disable sleep mode after waking.                    
}

void watchdogOn() { 
  MCUSR &= ~(1 << WDRF);                           // reset status flag
  WDTCSR |= (1 << WDCE) | (1 << WDE);              // enable configuration changes
  WDTCSR = (1<< WDP0) | (1 << WDP2); // set the prescalar = 5; 512ms
  WDTCSR |= (1 << WDIE);                           // enable interrupt mode
}

ISR(WDT_vect)
{
  sleep_count++; // keep track of how many sleep cycles have been completed.
}




