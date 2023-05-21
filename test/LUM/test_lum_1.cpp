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
  
  ini_Lum();


}

void loop() {

  float result =0.0;


  result=read_lum();
  Serial.println(result);
  delay(1000);


}