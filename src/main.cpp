// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor

#include <Adafruit_LSM6DSOX.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Mcp320x.h>
#include <LSM6DS0Sensor.h>
#include <BH1750.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include <WiFi.h>
#include "time.h"
#include "SPI.h"
// For SPI mode, we need a CS pin
#define SPI_CS    	5 		   // SPI slave select
#define ADC_VREF    5080     // 5V Vref
#define ADC_CLK     1600000  // SPI clock 1.6MHz
#define RESOLUTION_ADC 4096
#define O_APER 2530 //mV
#define SENSIVILIDAD_AMPER 1
const char* ssid       = "YOUR_SSID";
const char* password   = "YOUR_PASS";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
MCP3208 adc(ADC_VREF, SPI_CS);
bool primeraLecturaGiro= true;
float grados=0.0;
float lum = 0.0;
int zero = 2048;
Adafruit_LSM6DSOX sox;
BH1750 lightMeter(0x23);
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
String getTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  return (&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
void beginLocalTime()
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void ini_Lum(void){
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }

}

float read_Amper(uint16_t frequency, int pin) {
	uint32_t period = 1000000 / frequency;
	uint32_t t_start = micros();

	uint32_t Isum = 0, measurements_count = 0;
	int32_t Inow;

	while (micros() - t_start < period) {
		Inow = float(adc.toAnalog(adc.read(MCP3208::Channel::SINGLE_3))) - zero;
		Isum += Inow*Inow;
		measurements_count++;
	}

	float Irms = sqrt(Isum / measurements_count) / 4096 * 5.0 / 0.066;
	return Irms;
}
void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return "0";
  }

  Serial.print("Read from file: ");
  while(file.available()){
    return String(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
void iniSD(void){
  if(!SD.begin(4)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
}
float read_lum(void){
  float lux =0.0;
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
    return lux;
  }
  return lux;
}
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
  grados=readFile(SD,"/giro.txt").toInt();
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
  ini_Lum();
  iniSD();
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

  
  lum = read_lum();
  voltaje=read_Adc(0);
  voltaje=voltaje/1000;
  amperaje=read_Amper(2);

  viento= float(adc.toAnalog(adc.read(MCP3208::Channel::SINGLE_1)));
  if(viento>520){
    viento=viento/2000.0*32;
    viento= viento*1.61;
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
  lcd.print("kmh");
  lcd.print("   ");
  
  lcd.setCursor(0, 3);
  lcd.print("Luminosidad: ");
  lcd.print((float)lum,2);
  lcd.print("  ");


  String hora=getTime();
  String data_SD=hora+", Voltaje, "+String(voltaje)+", Amperaje, "+String(amperaje)+", viento, "+String(viento)+", Grados, "+String(grados);
  String giroAux= String(grados);
  appendFile(SD, "/data.txt",data_SD.c_str());
  writeFile(SD, "/giro.txt", giroAux.c_str());



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
