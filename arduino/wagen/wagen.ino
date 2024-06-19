#include <ArduinoMqttClient.h>
#include <WiFiS3.h>
#include "wifiSecret.h" // you need to create this file with your own network credentials (in .gitignore)
#include <Servo.h>
#include <FastLED.h>

// LEDs configuration
#define NUM_LEDS 6
#define DATA_PIN 9
CRGB leds[NUM_LEDS];

// Servo configuration
Servo servo1; 
Servo servo2;

// WiFi and MQTT configuration
char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS; 
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char broker[] = "192.168.50.160";
int port = 1883;
const char topic[] = "topic";

// Relay and control pin configurationint 
int R8 = 2;  // --> relay 8
int R7 = 3;  // --> relay 7
int R6 = 4;  // --> relay 6
int R5 = 5;  // --> relay 5
int R1 = 6;  // pomp --> relay 1
int zoemer =7;
int controle =8;

void setup() {
  // Initialize FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial) { }

  // Initialize pins
  pinMode(R8, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(zoemer, OUTPUT);
  pinMode(controle, OUTPUT);

  // Initialize servo
  servo1.attach(10);
  servo1.write(90);

  servo2.attach(11);
  servo2.write(90);

  // Connect to WiFi
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("You're connected to the network");

  // Connect to MQTT broker
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

  // Set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  // Subscribe to topic
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  mqttClient.subscribe(topic);
}

void loop() {
  mqttClient.poll();
}

void onMqttMessage(int messageSize) {
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  String messageContent = "";
  while (mqttClient.available()) {
    char c = (char)mqttClient.read();
    messageContent += c;
  }
  Serial.println(messageContent);

  int separatorIndex = messageContent.indexOf('/');
  String prefix = messageContent.substring(0, separatorIndex);
  String value = messageContent.substring(separatorIndex + 1);

  int extractedIntValue = 0;
  String extractedStringValue = "";

  if (prefix == "servo1") {
    extractedIntValue = value.toInt();
    Serial.print("Extracted Int Value: ");
    Serial.println(extractedIntValue);
    servo1.write(extractedIntValue);
  }else if(prefix== "servo2"){
  extractedIntValue = value.toInt();
    Serial.print("Extracted Int Value: ");
    Serial.println(extractedIntValue);
    servo2.write(extractedIntValue);
  
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
    } else if (extractedStringValue == "water") {
      Serial.println("Executing spuit");
      spuit();
    } else {
      Serial.println("Unknown command");
    }
    Serial.println();
  }
}

void linksachteruit() {
  digitalWrite(R8, HIGH); 
  digitalWrite(R7, LOW);
}

void rechtsachteruit() {
  digitalWrite(R6, LOW);
  digitalWrite(R5, HIGH);
}

void rechtsvooruit() {
  digitalWrite(R6, HIGH);
  digitalWrite(R5, LOW);
}

void linksvooruit() {
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

void links() {
  rechtsvooruit();
  Serial.println("links");
}

void rechts() {
  linksvooruit();
  Serial.println("rechts");
}

void spuit() {
  digitalWrite(R1, HIGH);
  delay(2000);
  digitalWrite(R1, LOW);
  Serial.println("spuit");
}
void sirene() {
  static unsigned long previousMillis = 0; 
  static bool sireneState = false; 
  const int interval = 500;
  const int duration = 5000; 

  unsigned long startMillis = millis();
  unsigned long currentMillis;

  while ((currentMillis = millis()) - startMillis <= duration) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (sireneState) {
        redLight();
      } else {
        blueLight();
      }
      sireneState = !sireneState;
    }
  }

  // Reset everything after 5 seconds
  resetLightsAndBuzzer();
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

void resetLightsAndBuzzer() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  digitalWrite(zoemer, LOW);
  FastLED.show();
}