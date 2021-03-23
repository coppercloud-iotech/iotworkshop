/* Sketch to send message to MQTT Broker on Cloud */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid        = "your-wifi-network-name";
const char* password    = "your-wifi-password";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// declare other variables, PINs, etc
/*

 */

void setup() {
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
}

void reconnect() {
   while (!mqttClient.connected()) {
      String clientId = String(ESP.getChipId());
      if (mqttClient.connect(clientId.c_str())) {Serial.println("connected");}
  }
}

void loop() {
  if (!mqttClient.connected()) {reconnect();}
  mqttClient.loop();
  String msg = "Test Message - Talking to MQTT";
  mqttClient.publish("outTopic", msg.c_str());
  delay(2000);
}
