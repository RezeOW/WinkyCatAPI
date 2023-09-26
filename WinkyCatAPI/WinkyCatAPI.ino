#include <WiFi.h>
#include <WebServer.h>
#include "WifiCreds.h"
#include "MQTTCreds.h"

// WiFi credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;

// Pin for Wink
const int Pin = 33;
const int PinConLoss = 27;

WebServer server(80);  // create a server on port 80

void setup() {
  Serial.begin(115200);
  pinMode(Pin, OUTPUT);
  pinMode(PinConLoss, OUTPUT); 
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  // Start the server
  server.on("/startWink", HTTP_POST, startWink);    // start
  server.on("/stopWink", HTTP_POST, stopWink);  // stop
  server.begin();  // start the server
}

void loop() {
  // Check for incoming connections
  server.handleClient();
  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("Lost Connection");
    ConnectionLoss();
  }
}

// Wink Handler

//Start
void startWink() {
  digitalWrite(Pin, HIGH);
  server.send(200);
  delay(10000);
  digitalWrite(Pin,LOW);
}

//Stop
void stopWink() {
  digitalWrite(Pin, LOW);
  server.send(200);
}

//Connection Handler

//Conection Retry
void ConnectionLoss() {
  digitalWrite(PinConLoss, HIGH);
  // Try to reconnect
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ConnectionRestored();
  Serial.println();
  Serial.println(WiFi.localIP());
}

// Reset Connection Led
void ConnectionRestored() {
  digitalWrite(PinConLoss, HIGH);
}
