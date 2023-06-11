#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter(0x23);
void ini_Lum(void){
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }

}
float read_lum(void){
  float lux =0.0;
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();

    return lux;
  }
  return lux;
}