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


void ini_bat(){
    julio_actual= readFile(SD,"/bat.txt").toFloat();

}
float bat_calc(double amperaje){
  julio_actual =julio_actual-amperaje*4.8;
  String jul_aux= String(julio_actual);

  writeFile(SD, "/bat.txt", jul_aux.c_str());
  float porcentaje = julio_actual*100/julio_total;
  return porcentaje;
}
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
  pinMode(4, OUTPUT);
     //end transfer
  ini_amper();
  ini_Adc();
  grados =ini_Giro();
  ini_lcd();
  ini_Lum();
  iniSD();
  begin_WiFi();
  timeClient.begin();
}

void loop() {
  check_version();
  double amper=0.0;
  float viento = 0.0;
  float lum = 0.0;
  float bat =0.0;




  grados = read_Giro(grados);


  Serial.println("grados");
  Serial.println(grados);



  viento = read_viento(1, settings );
  Serial.println("viento");
  Serial.println(viento);

  lum = read_lum();
  voltaje=read_volt(0, settings);
  Serial.println("lum");
  Serial.println(lum);
    Serial.println("voltaje");
    Serial.println(voltaje);



  
  amper=ISNS20_get_mA(settings);
    Serial.println("amper");
    Serial.println(amper);

  bat=bat_calc(amper);
    Serial.println("bat");
    Serial.println(bat);

  write_lcd((double)voltaje,"Voltaje: ","V", 1,2 );
  write_lcd((double)viento,"Viento: ","kmh", 2,2 );
  write_lcd((double)lum,"Luminosidad: ","", 3,2 );



  appendFile(SD, "/data.txt",(timeClient.getDay()+timeClient.getFormattedTime()+", "+String(voltaje)+", "+String(amper)+", "+String(viento)+", "+String(grados)+", "+String(lum)+", "+String(bat)+"/n").c_str());

  delay(10000);


}
