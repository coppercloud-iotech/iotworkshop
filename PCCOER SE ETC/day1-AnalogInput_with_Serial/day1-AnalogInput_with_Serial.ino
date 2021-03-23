
int sensorPin = A0;
int sensorValue = 0; 

void setup() {
  // initialize serial:
  Serial.begin(9600);
}

void loop() {
    sensorValue = analogRead(sensorPin);
    Serial.print("Sensor_value=");
    Serial.println(sensorValue);
    delay(1000);
}
