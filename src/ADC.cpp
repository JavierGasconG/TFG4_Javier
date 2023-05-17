#include <Arduino.h>
#include <SPI.h>
#include <Mcp320x.h>

#define SPI_CS    	5 		   // SPI slave select
#define ADC_VREF    5080     // 5V Vref

MCP3208 adc(ADC_VREF, SPI_CS);

void ini_Adc(void){

// configure PIN mode
  pinMode(SPI_CS, OUTPUT);

  // set initial PIN state



}
float read_Adc (int channel, SPISettings settings){
  digitalWrite(SPI_CS, HIGH);
  uint32_t t1;
  uint32_t t2;
  SPI.beginTransaction(settings);

  // start sampling
  uint16_t raw = 0; 
  t1 = micros();
  switch (channel)
  {
  case 0:
    raw = adc.read(MCP3208::Channel::SINGLE_0);
    break;
  case 1:
    raw = adc.read(MCP3208::Channel::SINGLE_1);
    break;
  case 2:
    raw = adc.read(MCP3208::Channel::SINGLE_2);
    break;
  case 3:
    raw = adc.read(MCP3208::Channel::SINGLE_3);
    break;
  case 4:
    raw = adc.read(MCP3208::Channel::SINGLE_4);
    break;
  case 5:
    raw = adc.read(MCP3208::Channel::SINGLE_5);
    break;

  case 6:
    raw = adc.read(MCP3208::Channel::SINGLE_6);
    break;
  case 7:
    raw = adc.read(MCP3208::Channel::SINGLE_7);
    break;

  default:
    raw = adc.read(MCP3208::Channel::SINGLE_0);

    break;
  }

  t2 = micros();

  // get analog value
  uint16_t val = adc.toAnalog(raw);
  SPI.endTransaction();
  digitalWrite(SPI_CS, LOW);
  return val;
}