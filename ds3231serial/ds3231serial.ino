#include "Wire.h"


#define DS3231_I2C_ADDRESS 0x68

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
char c='\0';
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int vals[7], i=0;

byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
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
void setRTCTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
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

String getTimeString()
{
  // retrieve data from DS3231
 readRTCTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 
 String time = "";
 if(dayOfMonth<10) { time = time + "0"+String(dayOfMonth);} else {time = String(dayOfMonth);}
 if(month<10) { time = time + '/' + "0"+String(month);} else {time = time + '/' +String(month);}
time = time + '/' +String(year);
time = time + " ";
if(hour<10) { time = time + "0"+String(hour);} else {time = time +String(hour);}
if(minute<10) { time = time + ':' + "0"+String(minute);} else {time = time + ':' +String(minute);}
if(second<10) { time = time + ':' + "0"+String(second);} else {time = time + ':' +String(second);}
  return time;
}



void setup() {
  // initialize serial for sink node
   inputString.reserve(30);
   Wire.begin();
   Serial.begin(38400);
 //  setRTCTime(50,10,5,6,22,4,17);
}


void loop() {
  
    if (stringComplete) 
    {
    // clear the string:
    inputString = "";
    stringComplete = false;
    setRTCTime(vals[0],vals[1],vals[2],vals[3],vals[4],vals[5],vals[6]);
        while(1)
        {
        Serial.println(getTimeString());
    	  delay(2000);
        }
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    if(inChar==',')
    {
      vals[i] = inputString.toInt();
      i++;
      inputString="";
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


