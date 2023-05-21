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
  
  iniSD();


}

void loop() {
  String result=""
  appendFile(SD, "/test.txt",("test")c.str);
  writeFile(SD, "/test2.txt",("test2")c.str);
  result =readFile(SD, "/test.txt");
  Serial.println(result);
  result=readFile(SD, "/test1.txt");
  Serial.println(result);

  delay(1000);


}