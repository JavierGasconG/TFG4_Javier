#include "LCD.h"
#include <Arduino.h>
#include "Lum.h"
#include <WiFi.h>
#include <micro_SD.h>
#include "time.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "ADC.h"
#include "OTA.h"
#include "Giro.h"
#include "Viento.h"
#include "Amper.h"
#include "Volt.h"
String serverName = "http://192.168.1.205/"; 

#include <HTTPClient.h>

const char* ssid       = "SBC";
const char* password   = "SBCwifi$";




void begin_WiFi(){
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.println ( "wifi" );
  }

}



void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 

  begin_WiFi();

}


void loop() {

    if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        Serial.println("Wifi conected");
        String serverPath = serverName + "test";
        http.begin(serverPath.c_str());
        int httpResponseCode = http.GET();
        if (httpResponseCode>0) {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
}