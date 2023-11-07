#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() { 

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("LM35D Texting...");
}

void loop() {

    int val;
    int dat;
    val=analogRead(1);//温度传感器接到模拟PIN0上；
    dat=(125*val)>>8;
    lcd.setCursor(0,1);
    lcd.print("Tep:    C");
    lcd.setCursor(4,1);
    lcd.print(dat);
    delay(200);
}
