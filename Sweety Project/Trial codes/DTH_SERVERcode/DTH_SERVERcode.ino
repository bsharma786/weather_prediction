// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>; 
#include <ThingSpeak.h>;
#include "DHT.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define DHTPIN D6     // what pin we're connected to
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
const char* ssid = "JioFiber-6HPFC"; //Your Network SSID
const char* password = "jionet123"; //Your Network Password
WiFiClient client;
unsigned long myChannelNumber = 1411111; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "R49YBNJI41BMLRK7"; //Your Write API Key
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600); 
 // Serial.println("DHTxx test!");
  dht.begin();
 WiFi.begin(ssid, password);
 ThingSpeak.begin(client);
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}
 
void loop() {
  // Wait a few seconds between measurements.
  delay(5000);
// display.clearDisplay();
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
   float hi = dht.computeHeatIndex(f, h);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  // Compute heat index
  // Must send in temp in Fahrenheit!
 
 
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
delay (200);
char temp_buff[5]; char hum_buff[5]; char P_buff[15]= "%" ; char C_buff[15]= "C";
    char temp_disp_buff[11] = "Tmp:";
    char hum_disp_buff[11] = "Hum:";
    
    dtostrf(t,2,1,temp_buff);
    strcat(temp_disp_buff,temp_buff);
    dtostrf(h,2,1,hum_buff);
    strcat(hum_disp_buff,hum_buff);
    
    // routine for displaying text for temp/hum readout
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(temp_disp_buff);
    display.print("\tC");
    display.setTextSize(2);
//    display.setTextColor(WHITE);
    display.setCursor(0,18);
    display.print(hum_disp_buff);
    display.print("%");
    display.display();
    delay(2000);
}
