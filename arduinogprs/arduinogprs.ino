
void printLine(String command, bool awaitResponse, String expectedResponse)
{
  String response="";
  if(awaitResponse)
  {
     Serial.print(command + "\r");
	 delay(200);
     while(Serial.available())
	 {
	   response+=Serial.read();
	   if(response.indexOf(expectedResponse) > -1)
	   {
	      //print response
	      break;
	   }
	 }
  }
  else
    Serial.print(command + "\r");
	delay(100);
}