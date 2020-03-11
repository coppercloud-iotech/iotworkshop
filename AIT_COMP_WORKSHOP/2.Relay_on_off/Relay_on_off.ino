int RELAY= D2;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RELAY, LOW);
  delay(1000);  
  // wait for a second
  digitalWrite(RELAY, HIGH);
  delay(1000);                       // wait for a second
}
