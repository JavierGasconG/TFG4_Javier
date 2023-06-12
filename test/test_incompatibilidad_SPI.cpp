
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

SPISettings settings(1000000, MSBFIRST, SPI_MODE0);





void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
  SPI.begin();
  ini_amper();
  ini_Adc();

  iniSD();

}

void loop() {
  double amper=0.0;
  amper=get_mA(settings);
  Serial.print(amper);
  Serial.println(" mA");

  float adc =0.0;

  adc=read_Adc(0,settings);
  Serial.println(adc);

  String result="";
  appendFile(SD, "/test.txt", "test");
  writeFile(SD, "/test2.txt", "test2");
  result =readFile(SD, "/test.txt");
  Serial.println(result);
  result=readFile(SD, "/test1.txt");
  Serial.println(result);
}
