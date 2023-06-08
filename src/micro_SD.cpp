#include <Arduino.h>
#include <SD.h>

void appendFile(fs::FS &fs, const char * path, const char * message){
    digitalWrite(4, LOW);
    //Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        //Serial.println("Failed to open file for appending");
        digitalWrite(4, HIGH);
        return;
    }
    if(file.println(message)){
        //Serial.println("Message appended");
    } else {
        //Serial.println("Append failed");
    }
    file.close();
    digitalWrite(4, HIGH);
}
String readFile(fs::FS &fs, const char * path){
  digitalWrite(4, LOW);
  //Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    //Serial.println("Failed to open file for reading");
    digitalWrite(4, HIGH);
    return "0";
  }

  //Serial.print("Read from file: ");
  String result="";
  while(file.available()){
    result=result+String((char)file.read());

  }
  
    file.close();
    digitalWrite(4, HIGH);

    return result;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  digitalWrite(4, LOW);
  //Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    //Serial.println("Failed to open file for writing");
    return;
  }
  if(file.println(message)){
    //Serial.println("File written");
  } else {
    //Serial.println("Write failed");
  }
  file.close();
    digitalWrite(4, HIGH);
}
void iniSD(void){
  digitalWrite(4, LOW);
  while(!SD.begin(4)){
    //Serial.println("Card Mount Failed");
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    //Serial.println("No SD card attached");
    return;
  }

  //Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    //Serial.println("MMC");
  } else if(cardType == CARD_SD){
    //Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    //Serial.println("SDHC");
  } else {
    //Serial.println("UNKNOWN");
  }
  digitalWrite(4, HIGH);
}
