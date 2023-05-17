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
#define RESOLUTION_ADC 4096
#define O_APER 2530 //mV
double voltaje=0.0;

#define SENSIVILIDAD_AMPER 1
SPISettings settings(1000000, MSBFIRST, SPI_MODE0);

const char* ssid       = "SBC";
const char* password   = "SBCwifi$";

const char* ntpServer = "pool.ntp.org";

float grados=0.0;



int zero = 2048;
int julio_total=41472;
int julio_actual=41472;

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
  SPI.begin();
  digitalWrite(4, HIGH);     //end transfer
  //ini_amper();
  ini_Adc();
  //grados =ini_Giro();
  //ini_lcd();
  //ini_Lum();
  //iniSD();
  //begin_WiFi();
  //timeClient.begin();
}

void loop() {
  //check_version();
  //double amper=0.0;
  //float amperaje=0.0;
  float viento = 0.0;
  //float lum = 0.0;

  //int i =0;

  //while (i<2)
  //{
  //  grados = read_Giro(grados);
  //  i++;
  //  write_lcd((double)grados, "Grados: ","", 0,2 );

  //}


  viento = read_viento(1, settings );
  
  //lum = read_lum();
  //voltaje=read_volt(0, settings);

  Serial.print(viento);
  Serial.println(" kmh");
  //pinMode(4, OUTPUT);
  //amper=ISNS20_get_mA(settings);
  //Serial.print(amper);
  //Serial.println(" A");
  delay(500);
  //amperaje=read_Amper(2);

  //write_lcd((double)voltaje,"Voltaje: ","V", 1,2 );
  //write_lcd((double)viento,"Viento: ","kmh", 2,2 );
  //write_lcd((double)lum,"Luminosidad: ","", 3,2 );

  //String giroAux= String(grados);

  //writeFile(SD, "/giro.txt", giroAux.c_str());

  //appendFile(SD, "/data.txt",(timeClient.getDay()+timeClient.getFormattedTime()+"/n"", Voltaje, "+String(voltaje)+", Amperaje, "+String(amperaje)+", viento, "+String(viento)+", Grados, "+String(grados)).c_str());
  //Serial.println(timeClient.getDay()+timeClient.getFormattedTime());



}
