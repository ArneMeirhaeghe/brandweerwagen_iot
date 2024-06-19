#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "wifiSecret.h" // you need to create this file with your own network credentials (in .gitignore)
 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
 
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
 
const char broker[] = "192.168.50.160";
int        port     = 1883;
const char topic[]  = "topic";
const char topic2[]  = "real_unique_topic_2";
const char topic3[]  = "real_unique_topic_3";

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int currentMoistureLevel;//Like   so: current/max*100 = Moisture level as a percentage

void setup() {
  pinMode (A0, INPUT); //A0   is the pin used for the Soil Moisture Sensor
  delay(2000);
  Serial.begin(9600);


  Serial.println("------------------");



  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(2000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  while (!mqttClient.connect(broker, port)) {
  Serial.print("MQTT connection failed! Error code = ");
  Serial.println(mqttClient.connectError());
  delay(1000); // wacht een seconde voordat je opnieuw probeert
}

Serial.println("You're connected to the MQTT broker");
Serial.println();


  Serial.println("You're connected to the MQTT broker!");
  Serial.println("------------------");
}

void   loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  currentMoistureLevel = analogRead(A0); //Keep renewing the readings   for the current moisture level


  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    Serial.println(currentMoistureLevel);

    mqttClient.beginMessage(topic2);
    mqttClient.print(currentMoistureLevel);
    mqttClient.endMessage();
    Serial.print("message send!");
    Serial.println();
  }
  delay(500); //Short   delay to not overload the program
}