/*
Project:  Output send over MQTT with a ESP8266 / NodeMCU
Author:   Thomas Edlinger for www.edistechlab.com
Date:     Created 19.04.2020 
Version:  V1.0
 
Required libraries (Tools -> manage libraries)
 - PubSubClient by Nick O'Leary V2.7.0  
Required Board (Tools -> Board -> Boards Manager...)
 - Board: esp8266 by ESP8266 Community V2.6.3

Wirering for the Output:
Relais      NodeMCU
VCC         3.3V
GND         G
SCL         D1 / GPIO5
SDA         D2 / GPIO4
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define wifi_ssid "MacWorld"
#define wifi_password "E2943228373F65C0"

#define mqtt_server "PiHomeserver.local"
#define mqtt_user "your_username"         
#define mqtt_password "your_password"     

//#define humidity_topic "esp01/humidity"
//#define temperature_topic "esp01/temperature"
//#define pressure_topic "esp01/pressure"
#define status_topic "esp01/status" 
#define inTopic "esp01/output"
    
  
WiFiClient espClient;  
PubSubClient client(espClient);  
   
bool status;  
const int OutputPin = 4;

long lastMsg = 0; ///----
char msg[50];     ///----
int value = 0;    ///----

    
 void setup() {  
  Serial.begin(115200);   
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback); 
  if (!client.connected()) {  
    reconnect();  
  }  
  pinMode(OutputPin, OUTPUT);
//  client.loop();   
  
 }  
   
 void loop() {  
 }  
   
 void setup_wifi() {  
  delay(10);  
  // We start by connecting to a WiFi network  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(wifi_ssid);  
  WiFi.begin(wifi_ssid, wifi_password);  
  while (WiFi.status() != WL_CONNECTED) {  
   Serial.print("."); 
   delay(500);  
  }  
  Serial.println("");  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  
 }  

  void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp01/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      //digitalWrite(ledPin, LOW);
    }
  }
}

 void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(status_topic, "ESP01 alive");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
   }
}
