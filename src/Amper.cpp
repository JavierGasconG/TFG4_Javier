/************************************************************************

  Test of Pmod ISNS20 & Pmod SSR

*************************************************************************

  Description: Pmod_ISNS20, Pmod_SSR
  The current through a load will be measured

  Material
  1. Arduino Uno
  2. Pmod ISNS20
  3. Pmod SSR

  Wiring
  ISNS20 <----------> Arduino <----------> SSR
  -         to        5V          to        V
  VCC       to        3V3         to        -
  GND       to        GND         to        G
  CS        to        10          to        -
  DO        to        12(MISO)    to        -
  CLK       to        13(SCK)     to        -
  -         to        2           to        ON

************************************************************************/

#include <SPI.h> // call library
#include <Arduino.h>
#define CS 2 //chip select pin
#define FILTER true //enable/disable digital filter
#define AVG_NUM 100 //number of readings to average (if filter is on)

double offset = -1.499; //variable to save measurement offset

double ISNS20_get_mA( SPISettings settings) {
  SPI.beginTransaction(settings);

  double sum = 0;
  for (int i = 0; i < AVG_NUM; i++) {
    int temporal = 0;
    digitalWrite(CS, LOW);      //begin SPI transfer
    delay(1);
    temporal = SPI.transfer(0x00);   //transfer first byte in
    temporal <<= 8; //shift msb to place
    delay(1);
    temporal |= SPI.transfer(0x00);   //transfer and append second byte in
    delay(1);
    digitalWrite(CS, HIGH);     //end transfer
    delay(1);
    double result = temporal / 4096.0 * (-3.0);  //convert raw value to mA: bit result / 12 bits * 3.0V reference


  
    result = (result - offset) / 0.066; //correct offset: (result-offset)/(0.066V per Amp ratio)
    
    //if no filtering is needed, return the result
    if (!FILTER) {
      return result;
    }

    sum += result;  //add partial results for averaging
  }
  sum /= AVG_NUM; //divide sum to get the average
  SPI.endTransaction();
  
  return sum;
}
void ini_amper() {


  
  pinMode(CS, OUTPUT);  //set chip select pin as output



}


//measure with the sensor (send true as parameter to measure the offset)
