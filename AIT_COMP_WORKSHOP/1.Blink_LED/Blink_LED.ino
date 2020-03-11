
int LED1= D0;
int LED2= D4;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  // wait for a second
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
