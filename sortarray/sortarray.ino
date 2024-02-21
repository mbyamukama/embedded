int m [] = {34,1,32,67,12,6,4};

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
sort(m,7);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i< (sizeof(m)/sizeof(int)); i++)
  {
    Serial.print(String(m[i]) + ",");
  }
  delay(1000);
  Serial.println(" ");
  if (Serial.available() > 0) {
                // read the incoming byte:
                int ine = Serial.read();
  int h = findnext(m, ine);
  Serial.println("Next item after " + String(ine) + " is " + String(h));
  delay(5000);
  }

}


void sort(int a[], int sizea) {
    for(int i=0; i<(sizea-1); i++) {
        for(int o=0; o<(sizea-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

int findnext(int a[], int n)
{
  for(int i=0; i<7; i++)
  {
    if(a[i]> n) //next scheduled arrival after current time
    {
      return a[i];
      break;
    }
  }
}
