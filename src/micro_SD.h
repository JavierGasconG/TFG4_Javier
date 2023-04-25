#include <Arduino.h>
#include<SD.h>
void appendFile(fs::FS &fs, const char * path, const char * message);
String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void iniSD(void);