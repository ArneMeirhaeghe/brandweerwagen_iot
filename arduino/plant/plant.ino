#include <WiFiNINA.h>
 #include "wifiSecret.h" // you need to create this file with your own network credentials (in .gitignore)

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;     // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket
 
WiFiClient client = server.available();
 
int R1 = 7;
int R2 = 9;
int R3 = 10;
int R4 = 11;
 
void setup() {
  Serial.begin(9600);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
 
 
  while (!Serial);
 
  enable_WiFi();
  connect_WiFi();
 
  server.begin();
  printWifiStatus();
 
}
 
void loop() {
  client = server.available();
 
  if (client) {
    printWEB();
  }
}
 
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
 
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
 
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
 
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
 
void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
 
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}
 
void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
 
    // wait 10 seconds for connection:
    delay(10000);
  }
}
 
void printWEB() {
 
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
 
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
           
            // Button to turn the LED on
        client.println("<form action=\"/V\" method=\"get\">");
        client.println("<button type=\"submit\">vooruit</button>");
        client.println("</form><br>");
 
 client.println("<form action=\"/S\" method=\"get\">");
        client.println("<button type=\"submit\">stop</button>");
        client.println("</form><br>");
 
        client.println("<form action=\"/A\" method=\"get\">");
        client.println("<button type=\"submit\">achteruit</button>");
        client.println("</form><br>");
        // Button to turn the LED off
       
           
            int randomReading = analogRead(A1);
            client.print("Random reading from analog pin: ");
            client.print(randomReading);
           
           
           
 
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
 
       if (currentLine.endsWith("GET /V")) {
           vooruit();        
}
   if (currentLine.endsWith("GET /S")) {
          stop();
}
   if (currentLine.endsWith("GET /A")) {
           achteruit();        
}
 
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
void vooruit() {
  digitalWrite(R1, LOW); // Zet de LED aan
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  Serial.println("vooruit");
}
 
void stop() {
  digitalWrite(R1, HIGH); // Zet de LED aan
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  Serial.println("STOP");
}
void achteruit() {
  digitalWrite(R1, HIGH); // Zet de LED aan
  digitalWrite(R2, LOW);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  Serial.println("achteruit");
}