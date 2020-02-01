#include <Servo.h>

Servo servo1;  
long num;     
long num1;
void setup()
{
 servo1.attach(7);
Serial.begin(9600); 
Serial.print("Enter1 Position = ");
}

void loop() 
{ 
  while(Serial.available()>0)
  { 
  num= Serial.parseInt(); 
L1:  Serial.print(num);Serial.println(" degree");
  Serial.print("Enter2 Position = ");
  //for(int i=0;i<=num;i++){
  servo1.write(num);
  delay(15);
  //}
K1:  while(Serial.available()>0){
  num= Serial.parseInt(); 
  if(num==0){
    delay(10);
    goto K1;
  } 
  else{goto L1;
  }
  }
  } 
}
