

#include <Wire.h>
#include <ESP8266WiFi.h>;
//#include <Adafruit_ADS1015.h>
//#include <Adafruit_MCP4725.h>
#include <WiFiClient.h>;

#include <ThingSpeak.h>;
Adafruit_MCP4725 dac;
Adafruit_ADS1115 ads(0x48);
const char* ssid = "JioFiber-6HPFC"; //Your Network SSID

const char* password = "jionet123"; //Your Network Password
WiFiClient client;

unsigned long myChannelNumber = 962628; //Your Channel Number (Without Brackets)

const char * myWriteAPIKey = "C63ULIZJRBCSE0X3"; //Your Write API Key

int val ; float v;
int A = 4700;
int potPin = A0; // Potentiostat Pin Connected at A0 Pin
void setup(void)
{
  Serial.begin(115200);
//  ads.begin();
    delay(10);

  // Connect to WiFi network
  WiFi.begin(ssid, password);



  ThingSpeak.begin(client);
}

void loop(void)
{
  
   
    v = (adc0 * 0.1875) / 1000;
     R = A*((v)/(5-v));
      
  Serial.print("AIN0: "); 
  Serial.print(adc0);
  Serial.print("\tVoltage: ");
  Serial.println(R, 7);
  Serial.println();     
  Serial.println(v, 3); //Print on Serial Monitor

    delay(1000);

    ThingSpeak.writeField(myChannelNumber, 1, R, myWriteAPIKey); //Update in ThingSpeak



    delay(100);



  
}
