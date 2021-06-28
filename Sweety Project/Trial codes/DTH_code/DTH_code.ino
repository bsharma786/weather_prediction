// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <Wire.h>
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>; 
#include <ThingSpeak.h>;
#include "DHT.h"

#define DHTPIN D1     // what pin we're connected to
 const char* ssid = "JioFiber-6HPFC"; //Your Network SSID
const char* password = "jionet123"; //Your Network Password
WiFiClient client;
unsigned long myChannelNumber = 1411111; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "R49YBNJI41BMLRK7"; //Your Write API Key
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  dht.begin();
 WiFi.begin(ssid, password);
 ThingSpeak.begin(client);
}
 
void loop() {
  // Wait a few seconds between measurements.
  delay(5000);
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
   
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
 
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");
ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey); //Update in ThingSpeak
delay (200);
ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey); //Update in ThingSpeak

}
