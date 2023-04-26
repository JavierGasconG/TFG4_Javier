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
#define CS 4 //chip select pin
#define RELAY 2 //solid state relay
#define FILTER true //enable/disable digital filter
#define AVG_NUM 100 //number of readings to average (if filter is on)

float offset = -1.49; //variable to save measurement offset

float ISNS20_get_mA(bool is_offset, SPISettings settings) {
  SPI.beginTransaction(settings);

  float sum = 0;
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
    float result = temporal / 4096.0 * (-3.0);  //convert raw value to mA: bit result / 12 bits * 3.0V reference

    //if the measurement isn't for offset, correct the error
    if (!is_offset) {
      result = (result - offset) / 0.066; //correct offset: (result-offset)/(0.066V per Amp ratio)
    }

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
void ini_amper(SPISettings settings) {

  //pinMode(RELAY, OUTPUT);   //initialize pin connected to relay
  //digitalWrite(RELAY, LOW); //turn the relay off (load not connected)

 
  
  pinMode(CS, OUTPUT);  //set chip select pin as output


  offset = ISNS20_get_mA(true, settings);  //measure and save the offset

  //digitalWrite(RELAY, HIGH);   //turn relay on
}


//measure with the sensor (send true as parameter to measure the offset)
