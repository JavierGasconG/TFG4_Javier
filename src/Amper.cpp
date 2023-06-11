

#include <SPI.h> 
#include <Arduino.h>
#define CS 2 //chip select pin
#define AVG_NUM 100 //number of readings to average 

double offset = -1.499; //variable to save measurement offset

double get_mA( SPISettings settings) {
  SPI.beginTransaction(settings);

  double sum = 0;
  for (int i = 0; i < AVG_NUM; i++) {
    int temporal = 0;
    digitalWrite(CS, LOW);      //begin SPI transfer
    delay(1);
    temporal = SPI.transfer(0x00);   
    temporal <<= 8; 
    delay(1);
    temporal |= SPI.transfer(0x00);   
    delay(1);
    digitalWrite(CS, HIGH);     //end transfer
    delay(1);
    double result = temporal / 4096.0 * (-3.0);  //convert raw value to mA: bit result / 12 bits * 3.0V reference


  
    result = (result - offset) / 0.066; //correct offset: (result-offset)/(0.066V per Amp ratio)
    
 

    sum += result;  
  }
  sum /= AVG_NUM; 
  SPI.endTransaction();
  
  return sum;
}
void ini_amper() {


  
  pinMode(CS, OUTPUT);  //set chip select pin as output



}

