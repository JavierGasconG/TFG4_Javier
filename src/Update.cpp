#include <WiFi.h>+
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
String serverName = "http://192.168.1.205/"; 
String version = "";
void check_version(void){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String serverPath = serverName + "version";
      http.begin(serverPath.c_str());
            int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        String payload = http.getString();

        Serial.print("version: ");
        Serial.println(payload);
        if(version != payload){
          t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.205/firmware.bin");       
        }
      }
  }
}