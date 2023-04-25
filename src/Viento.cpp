#include "ADC.h"
float read_viento(int channel){
  float viento=0.0;
  viento= read_Adc(channel);
  if(viento>520){
    viento=viento/2000.0*32;
    viento= viento*1.61;
  }else{
    viento =0.0;
  }
  return viento;
}