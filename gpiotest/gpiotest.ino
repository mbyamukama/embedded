
int ow2 = 15; // PD 6 ** 15 ** 
int pe7 = 8;
void setup() {
  // put your setup code here, to run once:
  pinMode(pe7, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pe7, HIGH);
  delay(3000);
   digitalWrite(pe7, LOW);
  delay(3000);
}
