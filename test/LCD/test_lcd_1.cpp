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



void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
  SPI.begin();
  
  ini_lcd();


}

void loop() {
  write_lcd((double)0.0,"test: ","0", 0,2 );

  write_lcd((double)1.0,"test: ","1", 1,2 );
  write_lcd((double)2.0,"test: ","2", 2,2 );
  write_lcd((double)3.0,"test: ","3", 3,2 );
  delay(1000);


}