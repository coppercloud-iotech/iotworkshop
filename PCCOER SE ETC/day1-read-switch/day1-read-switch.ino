/* Sketch to read state of a PIN */

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(04, INPUT);
  pinMode(16, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  int pinValue = digitalRead(04);             // this is how we read a PIN
  if(pinValue==LOW){digitalWrite(16,HIGH);}
  else{digitalWrite(16,LOW);}
  delay(100);
}
