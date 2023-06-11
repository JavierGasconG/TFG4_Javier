#include <Arduino.h>
#include "micro_SD.h"
#include <Adafruit_LSM6DSOX.h>

Adafruit_LSM6DSOX sox;

float ini_Giro(void){
  while (!sox.begin_I2C(0x6B)) {
    
    delay(10);
    Serial.println("error giro");
  }
  return readFile(SD,"/giro.txt").toFloat();
}

float read_Giro( float grados){
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  int i=0;
  float gradosRecogidos=0.0;
  while(i<10)
  {
    sox.getEvent(&accel, &gyro, &temp);
    gradosRecogidos = (gyro.gyro.z + 0.02) * 57.2958;
    if(gradosRecogidos > 0.10 || gradosRecogidos < -0.1 ){
      grados= grados - (((gyro.gyro.z + 0.02) * 57.2958)/10);
    

    }

    
    delay(100);
    i++;
  }
  String giroAux= String(grados);

  writeFile(SD, "/giro.txt", giroAux.c_str());
  return grados;

}
