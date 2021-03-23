/* Sketch to read ultrasound distance sensor and send the values to MQTT on cloud */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid        = "iot-coppercloud";
const char* password    = "coppercloud";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

char mqttUsername[20] = "iotcube";
char mqttPassword[20] = "i0tcub3";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

long lastMsg = 0;
char msg[200];
int value = 0;
char _msgLwt[256];
String msgPrefix = "";
uint32_t getDeviceID() {return getChipID();}
uint32_t getChipID() {return ESP.getChipId();}
const char* getClientID() {return String(getChipID()).c_str();}
String topicLwt = "lwt";
uint8_t connAttempts = 0;

const int TrigerPin = D5;
const int EchoPin = D6;
String message2 = "devicename:distancesensor,msgtype:lwt";

//const int RelayPin =  D2;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMqtt() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  // invoke "callback()" function when there is an incoming message
  mqttClient.setCallback(callback);   
  setStandardMessages();
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected() && (connAttempts < 5)) {  //each attempt waits for a long time already
        connAttempts++;
        mqttClient.connect(getClientID(), mqttUsername, mqttPassword, topicLwt.c_str(), 0, false, _msgLwt);
        delay(500);
    }
    String clientId = String(ESP.getChipId());
    Serial.println();
    Serial.println(clientId);
    if (mqttClient.connect(clientId.c_str())) {Serial.println("connected");delay(20);sendBirthMessage();}
}

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();       // setup WiFi connection
  setupMqtt();       // setup mqtt connection
}

void loop() {
  if (!mqttClient.connected()) {reconnect();}
  mqttClient.loop();
  //client.loop();
  readSensor();
  delay(100);
}

void readSensor(){
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(TrigerPin, OUTPUT);
  digitalWrite(TrigerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(TrigerPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(EchoPin, INPUT);
  duration = pulseIn(EchoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm     = microsecondsToCentimeters(duration);

  Serial.print(cm); Serial.print("cm"); Serial.println();
  String value = String(cm);
  String message = "";
  message = message + "{\"sensorid\":" + ESP.getChipId()
            + ",\"lat\":18.4423,\"long\":73.7671,\"value\":" + value + "}";
  mqttClient.publish("distancesensor", message.c_str());
  delay(1000);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void setStandardMessages() {
    // todo - replace String with c_str
    msgPrefix =String("\"deviceid\":\"") + getDeviceID() + "\",";
    
    // lwt message template
    String msg = "";
    msg = "{" + msgPrefix + String("\"msgtype\":\"lwt\"}");
    msg.toCharArray(_msgLwt, 256);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {Serial.print((char)payload[i]);}
  Serial.println();

  payload[length] = '\0'; // terminate the incoming list of charas as a String

  // act on the incoming command
  if(strcmp(((char *)payload), "on") == 0)  {digitalWrite(D4,LOW);}
  else if(strcmp(((char*)payload),"off") == 0){digitalWrite(D4,HIGH);}
  else{Serial.println("Unexpected message");}
}

void sendBirthMessage(){
  String birthmessage = "";
  birthmessage = birthmessage + "{\"deviceid\":\"" + ESP.getChipId()+ "\"," + String("\"msgtype\":\"birth\"}");
  mqttClient.publish("birth", birthmessage.c_str());
}
