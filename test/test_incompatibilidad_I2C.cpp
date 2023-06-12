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

float grados=0.0;
void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
  SPI.begin();
  iniSD();
  ini_Lum();
  ini_lcd();
  
  grados =ini_Giro();


}

void loop() {

  float result =0.0;


  result=read_lum();
  Serial.println(result);

  grados=read_Giro(grados);
  Serial.println(grados);
  write_lcd((double)0.0,"test: ","0", 0,2 );

  write_lcd((double)1.0,"test: ","1", 1,2 );
  write_lcd((double)2.0,"test: ","2", 2,2 );
  write_lcd((double)3.0,"test: ","3", 3,2 );

}