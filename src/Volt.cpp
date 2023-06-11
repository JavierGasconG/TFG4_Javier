#include "ADC.h"
#include "SPI.h"
#define AVG_NUM 10
double read_volt(int channer,  SPISettings settings){
  double sum=0.0;
  for (int i = 0; i < AVG_NUM; i++) {
    sum+=read_Adc(0,settings);
  }
  sum/=AVG_NUM;
  sum=sum/1000*5.567;
  return sum;

}