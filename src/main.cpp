// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor

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
#include "Update.h"
#include "Giro.h"
#include "Viento.h"
#define RESOLUTION_ADC 4096
#define O_APER 2530 //mV
#define SENSIVILIDAD_AMPER 1
const char* ssid       = "SBC";
const char* password   = "SBCwifi$";

const char* ntpServer = "pool.ntp.org";

float grados=0.0;



int zero = 2048;
int julio_total=41472;
int julio_actual=41472;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);



float bat(float amperaje){
  julio_actual =julio_actual-amperaje*4.8;
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


float read_Amper(uint16_t frequency, int pin) {
	uint32_t period = 1000000 / frequency;
	uint32_t t_start = micros();

	uint32_t Isum = 0, measurements_count = 0;
	int32_t Inow;

	while (micros() - t_start < period) {
		Inow = read_Adc(pin) - zero;
		Isum += Inow*Inow;
		measurements_count++;
	}

	float Irms = sqrt(Isum / measurements_count) / 4096 * 5.0 / 0.066;
	return Irms;
}
float read_Amper (int chanel){
  float voltaje, amperaje =0.0;
  for(int i=0; i<10;i++){
      voltaje = read_Adc(chanel)*(5.0 / 4096.0);
      amperaje=amperaje+ (voltaje-2.527)/0.066;
  }
  amperaje=amperaje/10;

  return amperaje;


  
}


void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); 
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
  //  /* Get a new normalized sensor event */
  float voltaje=0.0;
  float amperaje=0.0;
  float viento = 0.0;
  float lum = 0.0;

  int i =0;

  while (i<2)
  {
    grados = read_Giro(grados);
    i++;
    write_lcd((double)grados, "Grados: ","", 0,2 );

  }


  viento = read_viento(1);
  
  lum = read_lum();
  voltaje=read_Adc(0);
  voltaje=voltaje/1000;
  amperaje=read_Amper(2);

  write_lcd((double)voltaje,"Voltaje: ","V", 1,2 );
  write_lcd((double)viento,"Viento: ","kmh", 2,2 );
  write_lcd((double)lum,"Luminosidad: ","", 3,2 );








  String giroAux= String(grados);

  writeFile(SD, "/giro.txt", giroAux.c_str());

  appendFile(SD, "/data.txt",(timeClient.getDay()+timeClient.getFormattedTime()+"/n"", Voltaje, "+String(voltaje)+", Amperaje, "+String(amperaje)+", viento, "+String(viento)+", Grados, "+String(grados)).c_str());
  Serial.println(timeClient.getDay()+timeClient.getFormattedTime());



  //  // serial plotter friendly format

  //  Serial.print(temp.temperature);
  //  Serial.print(",");

  //  Serial.print(accel.acceleration.x);
  //  Serial.print(","); Serial.print(accel.acceleration.y);
  //  Serial.print(","); Serial.print(accel.acceleration.z);
  //  Serial.print(",");

  // Serial.print(gyro.gyro.x);
  // Serial.print(","); Serial.print(gyro.gyro.y);
  // Serial.print(","); Serial.print(gyro.gyro.z);
  // Serial.println();
  //  delayMicroseconds(10000);
  //hol
}
