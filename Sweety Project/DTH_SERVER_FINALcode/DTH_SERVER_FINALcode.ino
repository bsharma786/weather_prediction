
#include <Wire.h> //Used for I2C devices in this case the display
#include <SPI.h> // Display driver
#include <Adafruit_GFX.h> // Display driver
#include <Adafruit_SSD1306.h> // Display driver
#include <ESP8266WiFi.h>; // WiFi library
#include <WiFiClient.h>; // WiFi library
#include <ThingSpeak.h>; // Thingspeak server library
#include "DHT.h" // DHT 22 Sensor library
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define DHTPIN D6     // DHT 22 connected to digital pin 6
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire); // Dispaly pixel 
const char* ssid = "JioFiber-6HPFC"; //Your Network SSID
const char* password = "jionet123"; //Your Network Password
WiFiClient client;
unsigned long myChannelNumber = 1411111; //ThingSpeak Channel Number (Without Brackets)
const char * myWriteAPIKey = "R49YBNJI41BMLRK7"; //Write API Key

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}

void loop() {
    float h = dht.readHumidity(); // Humidity Value
  delay(2000);
  float t = dht.readTemperature(); // Temperature Value
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!"); // If sensor not detected
    return;
  }
  else
  {
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.println("Temperature: ");
  Serial.println(t);
  Serial.print(" *C ");

  char temp_buff[5]; // Variable array to store temperature data 
  char hum_buff[5]; // Variable array to store humidity data 
  char temp_disp_buff[11] = "Tmp:"; // Variable array to show temperature data in display 
  char hum_disp_buff[11] = "Hum:";// Variable array to show humidity data in display

  dtostrf(t, 2, 1, temp_buff); // converts into ASCII
  strcat(temp_disp_buff, temp_buff);
  dtostrf(h, 2, 1, hum_buff); // converts into ASCII
  strcat(hum_disp_buff, hum_buff);
  display.clearDisplay();  // routine for displaying text for temp/hum readout
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(temp_disp_buff);
  display.print("\tC");
  display.setCursor(0, 18);
  display.print(hum_disp_buff);
  display.print("%");
  display.display();
  int x = ThingSpeak.writeField(myChannelNumber, 1, h, myWriteAPIKey); //Update humidity value in ThingSpeak server
  if(x == 200){
    Serial.println("Channel 1 update successful.");
  }
  else{
    display.println("Problem updating channel 1. HTTP error code " + String(x));
  }
  delay (30000);
  int y = ThingSpeak.writeField(myChannelNumber, 2, t, myWriteAPIKey); //Update temperature value in ThingSpeak server
  if(y == 200){
    Serial.println("Channel 2 update successful.");
  }
  else{
    display.println("Problem updating channel 2. HTTP error code " + String(y));
  }  
    delay(30000);
}
}
