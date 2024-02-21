
//int randNumber = random(10);//random function to simulate reception of transmission signal, or no reception

int swc_batt = 8;//labelling digital pin 8 as battery charging switch
int swc_cap = 9;//labelling digital pin 9 as super-cap charging switch
int swd_cap = 11;//labelling digital pin 11 as super-cap discharge switch
int swd_batt = 10;//labelling digital pin 10 as battery discharge switch
int sw_panel = 12;//labelling digital pin 12 as solar panel control switch

int st_panel, st_swc_cap, st_swd_cap, st_swc_batt, st_swd_batt;  //switch statuses

double v_panel=0, v_batt=0, v_cap=0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int report_int = 20000;  //20s report interval over serial interface



void setup() {
  // put your setup code here, to run once:
  pinMode(sw_panel, OUTPUT);//assigning digital pins as outputs for controlling various relay switches
  pinMode(swd_cap, OUTPUT);
  pinMode(swd_batt, OUTPUT);
  pinMode(swc_cap, OUTPUT);
  pinMode(swc_batt, OUTPUT);
  
  Serial.begin(9600);
  
  digitalWrite(sw_panel, HIGH); //panel ON at the beginning
  digitalWrite(swc_cap, LOW); //charge on, NC relay
  digitalWrite(swd_cap, LOW); //discharge off, NO relay
  digitalWrite(swc_batt, LOW);  
  digitalWrite(swd_batt, LOW); 
  
  st_panel=1;   //panel is ON
  st_swc_cap=1;  //cap charge is ON
  
  /***************************IMPLEMENT ALL SWITCH STATUSES HERE**********************/
  
  
}

void loop() {
  // measure voltages  
  v_panel = analogRead(A0);
  v_batt = analogRead(A1);
  v_cap = analogRead(A2);

if(v_panel < 1.5)  //arbitrary: can be changed. Boost converts from 0.9V but 1.5V is a warning.
{
  //if(it is day)
    Serial.println("V_PANEL LOW : " + String(v_panel));
    
    //the only thing we can do is discharge. set discharge  /***************************IMPLEMENT ALL SWITCH STATUSES HERE**********************/
    if(v_batt <=2.8 && v_cap >2.4) //discharge from capacitor
    {
      digitalWrite(swd_cap, HIGH); 
    }
    else if(v_batt >2.8 && v_cap <=2.4) //discharge from battery
    {
      digitalWrite(swd_batt, HIGH); 
    }
    else if(v_batt > 2.8 && v_cap >2.4) //we have both available. Choose. This condition will eventually trigger one of the above conditions. if say v_batt falls to 2.8, condition 1 will become the default.
    {
      digitalWrite(swd_batt, HIGH); 
    }
    else //NOTHING IS AVAILABLE! ERROR.
    {
      //SHUTDOWN??
      // SLEEP?
    }
    
}
else // we have an output from solar panel. we can charge and discharge
{
    if(v_batt <= 2.8 && v_cap >=3.6) //If battery voltage is below threshold and cap charged. Charge battery  /***************************IMPLEMENT ALL SWITCH STATUSES HERE**********************/
    {
      digitalWrite(swc_batt, LOW); 
      digitalWrite(swd_batt, LOW); //do not discharge battery
      digitalWrite(swd_cap,  HIGH); //discharge cap
    }
    if(v_batt >= 3.6 && v_cap <=2.4) //If battery voltage is high. Cap is discharged. Charge cap
    {
      digitalWrite(swc_cap, LOW);
      digitalWrite(swd_cap, LOW);  //do not discharge cap
      digitalWrite(swd_batt,HIGH); //discharge batt
    }
    if(v_batt >= 3.6 && v_cap >=3.6) //all charged. solar is available. choose. better to save battery and use cap
    {
      digitalWrite(swc_batt, LOW); 
      digitalWrite(swd_batt, LOW); //do not discharge battery
      digitalWrite(swd_cap,  HIGH); //discharge cap
      
    }
    if(v_batt <= 2.8 && v_cap <=2.4) //all discharged. solar is available. Choose. This condition will eventually trigger one of the above.
    {
      digitalWrite(swc_cap, LOW);  
    }
    
   }
   
   if(stringComplete) // a full command has come thru
   {
     if(inputString=="SS")  //system status
     {
       //SEND VOLTAGES AND SWITCH INFORMATION
     }
     if(inputString=="SYSOFF")  //system off
     {
        //TURN ALL RELAYS OFF
     }
     if(inputString=="SYSON")  //system on
     {
       //TURN ALL RELAYS ON
     }
     if(inputString=="CRI")  //change report interval. each subsequent command halves the interval until its less than 500ms and it is reset to 20s
     {
       report_int = (int)(report_int*0.5);
       if(report_int < 0.5)
       {
         report_int = 20;
       }
     }
   }
     
     delay(report_int);
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
