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


SPISettings settings(1000000, MSBFIRST, SPI_MODE0);


void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
  SPI.begin();
  
  ini_Adc();


}

void loop() {

  float adc =0.0;

  adc=read_Adc(0,settings);
  Serial.println(adc);
  delay(1000);


}