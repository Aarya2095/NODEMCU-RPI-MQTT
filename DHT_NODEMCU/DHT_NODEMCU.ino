#include "DHT.h"        
#define DHTTYPE DHT11   

#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 
#include <ESP8266WiFi.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 

//Wifi Access Point
#define WLAN_SSID       "Tech_D0017220" 
#define WLAN_PASS       "YRJUUTJG" 
#define MQTT_SERVER      "192.168.0.31"  // give static address
#define MQTT_PORT         1883                    
#define MQTT_USERNAME    "" 
#define MQTT_PASSWORD    "" 



// Create an esp8266 wificlient  to connect to the MQTT server. 
WiFiClient client; 

// Setup the mqtt client class

Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
/****************************** Feeds ***************************************/ 
// Setup a feed  for publishing. 

Adafruit_MQTT_Publish dht_led = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/leds/dht"); 

// Setup a feed for subscribtion
Adafruit_MQTT_Subscribe esp8266_led = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/leds/esp8266"); 
/*************************** Sketch Code ************************************/ 
void MQTT_connect(); 

void setup() { 
 Serial.begin(9600); 
 delay(10); 
 Serial.println(F("RPi-ESP-MQTT")); 
 
 // Connect to wifi access point. 
 Serial.println(); Serial.println(); 
 Serial.print("Connecting to "); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print("."); 

     dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature\n\n");
  delay(700);
  
 } 
 Serial.println(); 
 Serial.println("WiFi connected"); 
 Serial.println("IP address: "); Serial.println(WiFi.localIP()); 
 
 // Setup MQTT subscription for esp8266_led feed. 
 mqtt.subscribe(&esp8266_led); 
 
  delay(3000);
} 


void loop() { 
   
 MQTT_connect(); 

 Adafruit_MQTT_Subscribe *subscription; 
 while ((subscription = mqtt.readSubscription())) { 
   if (subscription == &esp8266_led) { 
     Serial.print(F("Got: ")); 
     Serial.println((char *)esp8266_led.lastread); 

     /*  */
 } 
 }

float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
  delay(800);
  
    String temperature = String(t);
  String humidity = String(h);


  // Prepare a JSON payload string
  String payload = "";
  payload += "\"temperature\":"; payload += temperature; payload += ",";
  payload += "\"humidity\":"; payload += humidity;
  payload += " ";
char attributes[100];
  payload.toCharArray( attributes, 100 );
    
    if (! (dht_led.publish(attributes) ))
    {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
   }
    
}
// Function to connect and reconnect as necessary to the MQTT server. 

void MQTT_connect() { 
 int8_t ret; 
 
 // Stop if already connected. 
 
 if (mqtt.connected()) { 
   return; 
 } 
 
 Serial.print("Connecting to MQTT... "); 
 uint8_t retries = 3; 
 while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected 
      Serial.println(mqtt.connectErrorString(ret)); 
      Serial.println("Retrying MQTT connection in 5 seconds..."); 
      mqtt.disconnect(); 
      delay(5000);  // wait 5 seconds 
      retries--; 
      if (retries == 0) { 
        // basically die and wait for WDT to reset me 
        while (1); 
      } 
 } 
 Serial.println("MQTT Connected!"); 
}  
