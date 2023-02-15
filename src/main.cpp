// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor

#include <Adafruit_LSM6DSOX.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Mcp320x.h>
#include <LSM6DS0Sensor.h>

// For SPI mode, we need a CS pin
#define SPI_CS    	5 		   // SPI slave select
#define ADC_VREF    5080     // 5V Vref
#define ADC_CLK     1600000  // SPI clock 1.6MHz
#define RESOLUTION_ADC 4096
#define O_APER 2530 //mV
#define SENSIVILIDAD_AMPER 1
MCP3208 adc(ADC_VREF, SPI_CS);
bool primeraLecturaGiro= true;
float grados=0.0;
Adafruit_LSM6DSOX sox;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void ini_Adc(void){
// configure PIN mode
  pinMode(SPI_CS, OUTPUT);

  // set initial PIN state
  digitalWrite(SPI_CS, HIGH);

  // initialize serial
  

  // initialize SPI interface for MCP3208
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
}
void ini_Giro(void){
  if (!sox.begin_I2C(0x6B)) {
    // if (!sox.begin_SPI(LSM_CS)) {
    // if (!sox.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    // Serial.println("Failed to find LSM6DSOX chip");
    while (1) {
      delay(10);
    }
  }
}
void ini_lcd(void){
  lcd.begin();                      // initialize the lcd 
  lcd.backlight();
}
void read_Giro( void){
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
      if (primeraLecturaGiro)
      {
        grados=0.0;
        primeraLecturaGiro = false;
      }
      
    }
    
    delay(100);
    i++;
  }

}
float read_Adc (int chanel){
  uint32_t t1;
  uint32_t t2;

  // start sampling
  uint16_t raw = 0; 
  t1 = micros();
  switch (chanel)
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

  return val;
}
float read_Amper (int chanel){
  float voltaje, amperaje =0.0;
  voltaje = read_Adc(chanel);
  amperaje= voltaje-O_APER/SENSIVILIDAD_AMPER;
  Serial.println(voltaje);
  Serial.println(amperaje);
  return amperaje;
}
void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  ini_Adc();
  ini_Giro();
  ini_lcd();
}

void loop() {

  //  /* Get a new normalized sensor event */
  float voltaje=0.0;
  float amperaje=0.0;
  float viento = 0.0;

  int i =0;
  while (i<2)
  {
    read_Giro();
    i++;
    lcd.setCursor(0, 0);
    lcd.print("Grados: ");
    lcd.print((double)grados,2);
    lcd.print("   ");
  }
  
  voltaje=read_Adc(0);
  voltaje=voltaje/1000;
  amperaje=read_Amper(2);

  viento= float(adc.toAnalog(adc.read(MCP3208::Channel::SINGLE_1)));
  if(viento>520){
    viento=viento/2000.0*32;
  }else{
    viento =0.0;
  }


  lcd.setCursor(0, 1);
  lcd.print("Voltaje: ");
  lcd.print((double)voltaje,2);
  lcd.print("V");
  lcd.print("   ");
  lcd.setCursor(0, 2);
  lcd.print("viento: ");
  lcd.print((double)viento,2);
  lcd.print("mph");
  lcd.print("   ");
  
  lcd.setCursor(0, 3);
  lcd.print("Amper: ");
  lcd.print((double)amperaje,2);
  lcd.print("mA");
  lcd.print("   ");




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
