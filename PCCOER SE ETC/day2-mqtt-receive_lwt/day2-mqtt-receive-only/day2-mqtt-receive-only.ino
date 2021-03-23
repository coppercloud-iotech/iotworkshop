/* Sketch to receive message from MQTT Broker on Cloud */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid        = "iot-coppercloud";
const char* password    = "coppercloud";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;
char mqttUsername[20] = "iotcube";
char mqttPassword[20] = "i0tcub3";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// declare other variables, PINs, etc
/*

 */
char _msgLwt[256];
String msgPrefix = "";
uint32_t getDeviceID() {return getChipID();}
uint32_t getChipID() {return ESP.getChipId();}
const char* getClientID() {return String(getChipID()).c_str();}
long    getRSSI() {return WiFi.RSSI();}
String topicLwt = "lwt";
uint8_t connAttempts = 0;

void setup() {
  pinMode(D4, OUTPUT);
  Serial.begin(115200);
  setupWifi();       // setup WiFi connection
  setupMqtt();       // setup mqtt connection
}

void setupWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500); Serial.print(".");}
}

void setupMqtt() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  // invoke "callback()" function when there is an incoming message
  mqttClient.setCallback(callback);   
  setStandardMessages();
}

void reconnect() {
//   while (!mqttClient.connected()) {
//      String clientId = String(ESP.getChipId());
//      if (mqttClient.connect(clientId.c_str())) {Serial.println("connected");}
//      // subscribe to an MQTT topic     
//      mqttClient.subscribe("inTopic");
//  }
while (!mqttClient.connected() && (connAttempts < 10)) {  //each attempt waits for a long time already
        connAttempts++;
        mqttClient.connect(getClientID(), mqttUsername, mqttPassword, topicLwt.c_str(), 0, false, _msgLwt);
        delay(500);
    }
    String clientId = String(ESP.getChipId());
    Serial.println();
    Serial.println(clientId);
    if (mqttClient.connect(clientId.c_str())) {Serial.println("connected");delay(20);sendBirthMessage();}
      // subscribe to an MQTT topic     
      mqttClient.subscribe("inTopic");
}

void loop() {
  if (!mqttClient.connected()) {reconnect();}
  mqttClient.loop();
  String msg2 = "Test Message - Talking to MQTT";
  mqttClient.publish("outTopic", msg2.c_str());
  delay(200);
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

void setStandardMessages() {
    // todo - replace String with c_str
    msgPrefix =String("\"deviceid\":\"") + getDeviceID() + "\",";
    
    // lwt message template
    String msg = "";
    msg = "{" + msgPrefix + String("\"msgtype\":\"lwt\"}");
    msg.toCharArray(_msgLwt, 256);
}

void sendBirthMessage(){
  String birthmessage = "";
  birthmessage = birthmessage + "{\"deviceid\":\"" + ESP.getChipId()+String("\",\"RSSI\":\"") + getRSSI() + "\"," + String("\"msgtype\":\"birth\"}");
  mqttClient.publish("birth", birthmessage.c_str());
}
