#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "wifiSecret.h" // you need to create this file with your own network credentials (in .gitignore)
 #include <Servo.h> 

 Servo myservo;
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

int R8 = 2;  // --> relay 8
int R7 = 3;  // --> relay 7
int R6 = 4;  // --> relay 6
int R5 = 5;  // --> relay 5
int R1 = 6;  // pomp --> relay 1
int zoemer =7;
int controle =8;
int servo = 9;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
   pinMode(R8, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(zoemer, OUTPUT);
  pinMode(controle,OUTPUT);
  myservo.attach(servo);
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

    while (1);
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
  mqttClient.subscribe(topic2);
  mqttClient.subscribe(topic3);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Topic: ");
  Serial.println(topic2);
  Serial.print("Topic: ");
  Serial.println(topic3);

  Serial.println();
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
    messageContent += c;  // Append the character to our message content string
  }



  // Print the full message content
  Serial.println(messageContent);
  // Extract the relevant part of the message
  int separatorIndex = messageContent.indexOf('/');
  String prefix = messageContent.substring(0, separatorIndex);
  String value = messageContent.substring(separatorIndex + 1);

  int extractedIntValue = 0;
  String extractedStringValue = "";

   if (prefix == "servo") {
    extractedIntValue = value.toInt();
    Serial.print("Extracted Int Value: ");
    Serial.println(extractedIntValue);

    myservo.write(extractedIntValue);



  } else if (prefix == "motor") {
    extractedStringValue = value;
    Serial.print("Extracted String Value: ");
    Serial.println(extractedStringValue);


    if (extractedStringValue == "vooruit") {
    Serial.println("Executing vooruit");
    vooruit();
  } else if (extractedStringValue == "achteruit") {
    Serial.println("Executing achteruit");
    achteruit();
  } else if (extractedStringValue == "links") {
    Serial.println("Executing links");
    links();
  } else if (extractedStringValue == "rechts") {
    Serial.println("Executing rechts");
    rechts();
  } else if (extractedStringValue == "stop") {
    Serial.println("Executing stop");
    stop();
  } else if (extractedStringValue == "sirene") {
    Serial.println("Executing sirene");
    sirene();
  }
  Serial.println();
}
  }



void linksachteruit(){
  digitalWrite(R8, HIGH); 
  digitalWrite(R7, LOW);
  //    digitalWrite(R3, LOW);  
  //  digitalWrite(R4, LOW);  

}
void rechtsachteruit(){
  digitalWrite(R6, LOW);
  digitalWrite(R5, HIGH);
}
void rechtsvooruit(){
digitalWrite(R6, HIGH);
  digitalWrite(R5, LOW);
}
void linksvooruit(){
     digitalWrite(R8, LOW);  
  digitalWrite(R7, HIGH);
}

 
void stop() {
  digitalWrite(R8, LOW); 
  digitalWrite(R7, LOW);
  digitalWrite(R6, LOW);
  digitalWrite(R5, LOW);
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
 rechtsvooruit();
  Serial.println("links");
   delay(5000);
    stop();
}
void rechts(){
linksvooruit();

  Serial.println("rechts");
  delay(5000);
 stop();
}
void spuit(){
  digitalWrite(R1, HIGH);
  delay(2000);
  digitalWrite(R1, LOW);
  Serial.println("spuit");
}
void sirene(){
    digitalWrite(zoemer,HIGH);
    delay(1000);
    digitalWrite(zoemer,LOW);
    delay(1000);
    Serial.println("sirene");

}