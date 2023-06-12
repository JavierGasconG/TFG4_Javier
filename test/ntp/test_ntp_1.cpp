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


double voltaje=0.0;

SPISettings settings(1000000, MSBFIRST, SPI_MODE0);

const char* ssid       = "SBC";
const char* password   = "SBCwifi$";

const char* ntpServer = "pool.ntp.org";

float grados=0.0;



int zero = 2048;
int julio_total=41472;
float julio_actual=41472;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);



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
  timeClient.begin();
}


void loop() {
  timeClient.forceUpdate();
  Serial.println(timeClient.getDay()+", "+timeClient.getFormattedTime());

}