#include <Arduino.h>
#include "micro_SD.h"
#include <Adafruit_LSM6DSOX.h>

Adafruit_LSM6DSOX sox;

float ini_Giro(void){
  if (!sox.begin_I2C(0x6B)) {
    while (1) {
      delay(10);
    }
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
    if(gradosRecogidos > 0.30 || gradosRecogidos < 0.1 ){
      grados= grados - (((gyro.gyro.z + 0.02) * 57.2958)/10);

      
    }
    
    delay(100);
    i++;
  }
  return grados;

}
