#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  
void write_lcd(double value, String message, String message_2, int line, int decimal = 2){
  if (line<4 and line>=0){
    lcd.setCursor(0, line);
    lcd.print(message);
    lcd.print((double)value,decimal);
    lcd.print(message_2);
    lcd.print("   ");
  }
}void ini_lcd(void){
  lcd.begin();                      // initialize the lcd 
  lcd.backlight();
}