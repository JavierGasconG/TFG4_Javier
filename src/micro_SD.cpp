#include <Arduino.h>
#include <SD.h>

void appendFile(fs::FS &fs, const char * path, const char * message){
    digitalWrite(4, LOW);
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        digitalWrite(4, HIGH);
        return;
    }
    file.println(message);
 
    file.close();
    digitalWrite(4, HIGH);
}
String readFile(fs::FS &fs, const char * path){
  digitalWrite(4, LOW);

  File file = fs.open(path);
  if(!file){
    digitalWrite(4, HIGH);
    return "0";
  }


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


  File file = fs.open(path, FILE_WRITE);
  if(!file){
    digitalWrite(4, HIGH);

    return;
  }
  file.println(message);

  file.close();
  digitalWrite(4, HIGH);
}
void iniSD(void){
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  while(!SD.begin(4)){
    Serial.println("Card Mount Failed");
  }

  digitalWrite(4, HIGH);
}
