#include "ADC.h"
#include <SPI.h>
float read_viento(int channel, SPISettings settings){
  float viento=0.0;
  viento= read_Adc(channel, settings);
  if(viento>520){
    viento=viento/2000.0*32;
    viento= viento*3.6;
  }else{
    viento =0.0;
  }
  return viento;
}