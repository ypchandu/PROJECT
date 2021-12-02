#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay_light            D1
#define Relay_fan              D2
#define Relay_iron             D3
#define WLAN_SSID       "OMIN"             
#define WLAN_PASS       "premChandu666"        
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "Ominpc"            
#define AIO_KEY        "aio_DTmE55TyKdFbjIdf03yUzlMjOi4L"   
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_light"); 
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay_fan");
Adafruit_MQTT_Subscribe iron = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay_iron");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay_light, OUTPUT);
  pinMode(Relay_fan, OUTPUT);
  pinMode(Relay_iron, OUTPUT);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Light);
  mqtt.subscribe(&fan);
  mqtt.subscribe(&iron);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light.lastread);
      int Light_State = atoi((char *)Light.lastread);
      digitalWrite(Relay_light, Light_State);
      
    }
    if (subscription == &fan) {
      Serial.print(F("Got: "));
      Serial.println((char *)fan.lastread);
      int fan_State = atoi((char *)fan.lastread);
      digitalWrite(Relay_fan, fan_State);
    }
    if (subscription == &iron) {
      Serial.print(F("Got: "));
      Serial.println((char *)iron.lastread);
      int iron_State = atoi((char *)iron.lastread);
      digitalWrite(Relay_iron, iron_State);
    }
  }

  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
