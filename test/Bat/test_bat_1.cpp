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
int julio_total=41472;
float julio_actual=41472;

void ini_bat(){
    julio_actual= readFile(SD,"/bat.txt").toFloat();

}
float bat_calc(double amperaje){
  julio_actual =julio_actual-amperaje*5.2;
  String jul_aux= String(julio_actual);

  writeFile(SD, "/bat.txt", jul_aux.c_str());
  float porcentaje = julio_actual*100/julio_total;
  return porcentaje;
}
void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
  SPI.begin();
  
  ini_bat();
}



void loop() {

  double result =0.0;


  result=bat_calc(50);
  Serial.println(result);
  result=bat_calc(0);
  Serial.println(result);
  result=bat_calc(-50);
  Serial.println(result);
  delay(1000);


}