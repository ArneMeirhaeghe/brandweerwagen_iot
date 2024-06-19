#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "wifiSecret.h" // you need to create this file with your own network credentials (in .gitignore)
 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.50.160";
int port = 1883;
const char topic[] = "topic";
// const char topic2[]  = "real_unique_topic_2";
// const char topic3[]  = "real_unique_topic_3";

int R1 = 2;
int R2 = 3;
int R3 = 4;
int R4 = 5;
int R5 = 6;
int zoemer =7;
int controle =8;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
   pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(zoemer, OUTPUT);
  pinMode(controle,OUTPUT);
  while (!Serial) {
  }
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    digitalWrite(controle, LOW);

    Serial.println(mqttClient.connectError());

    while (1)
      ;
  }

  Serial.println("You're connected to the MQTT broker!");
  digitalWrite(controle, HIGH);

  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);
  // mqttClient.subscribe(topic2);
  // mqttClient.subscribe(topic3);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Topic: ");
  Serial.println(topic);
  // Serial.print("Topic: ");
  // Serial.println(topic2);
  // Serial.print("Topic: ");
  // Serial.println(topic3);

  Serial.println();

  //leds
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alive which avoids being disconnected by the broker
  mqttClient.poll();
}

void onMqttMessage(int messageSize) {
  // Announce the receipt of a message and its topic
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // Initialize an empty string to store the message
  String messageContent = "";

  // Read the whole message
  while (mqttClient.available()) {
    char c = (char)mqttClient.read();  // Read a character from the client
    messageContent += c;               // Append the character to our message content string
  }



  // Print the full message content
  Serial.println(messageContent);

  // Compare the received message to "1"
  if (messageContent == "vooruit") {
    Serial.println("vooruit");
    vooruit();
  }else if(messageContent == "achteruit"){
    Serial.println("achteruit");
    achteruit();
  }else if(messageContent == "links"){
    Serial.println("links");
    links();
  }else if(messageContent == "rechts"){
    Serial.println("rechts");
    rechts();
  }else if(messageContent == "stop"){
    Serial.println("stop");
    stop();
  }else if(messageContent == "sirene"){
    Serial.println("sirene");
    sirene();
  }

  Serial.println(); // Print a newline for better separation of messages
}
void linksvooruit(){
  digitalWrite(R1, HIGH); 
  digitalWrite(R2, LOW);
  //    digitalWrite(R3, LOW);  
  //  digitalWrite(R4, LOW);  

}
void rechtsvooruit(){
  digitalWrite(R3, LOW);
  digitalWrite(R4, HIGH);
}
void rechtsachteruit(){
digitalWrite(R3, HIGH);
  digitalWrite(R4, LOW);
}
void linksachteruit(){
     digitalWrite(R1, LOW);  
  digitalWrite(R2, HIGH);
}


void stop() {
  digitalWrite(R1, LOW); 
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);
  Serial.println("STOP");
}
void vooruit() {
  rechtsvooruit();
  linksvooruit();
  Serial.println("vooruit");
}
void achteruit() {
  rechtsachteruit();
  linksachteruit();
  Serial.println("achteruit");
}
void links(){
 linksachteruit();
 rechtsvooruit();
  Serial.println("links");
}
void rechts(){
linksvooruit();
rechtsachteruit();
  Serial.println("rechts");
}
void spuit(){
  digitalWrite(R5, HIGH);
  delay(2000);
  digitalWrite(R1, LOW);
  Serial.println("spuit");
}


void sirene() {

  static int sireneCount = 0;               // Teller om het aantal sirene-activeringen bij te houden
  static unsigned long previousMillis = 0;  // Houdt de laatste tijdsmarkering bij
  static bool sireneState = false;          // Houdt bij of de sirene aan of uit is
  const int interval = 500;
  const int tenSec = 10000;  // Interval tussen schakelingen in milliseconden

  unsigned long startMillis = millis();
  unsigned long startMillisLight = startMillis;
  redLight();
  // Controleer of het interval is verstreken
  if (millis() - startMillis <= tenSec) {
    //  previousMillis = currentMillis; // Reset de tijdsmarkering
    if (millis() - startMillisLight >= interval) {
      if (sireneState) {
        redLight();
      } else {
        blueLight();
      }
      sireneState != sireneState;
      startMillisLight = millis();
    }
  }
}


void blueLight() {
  leds[0] = CRGB::Blue;
  leds[1] = CRGB::Blue;
  leds[2] = CRGB::Red;
  leds[3] = CRGB::Red;
  leds[4] = CRGB::Blue;
  leds[5] = CRGB::Blue;
  digitalWrite(zoemer, LOW);
  FastLED.show();
}

void redLight() {
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Red;
  leds[2] = CRGB::Blue;
  leds[3] = CRGB::Blue;
  leds[4] = CRGB::Red;
  leds[5] = CRGB::Red;
  digitalWrite(zoemer, HIGH);
  FastLED.show();
}
