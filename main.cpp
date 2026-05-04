#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// This is the code for my desktop buddy, my main goal is to have a small display that can show the time, date, weather, and maybe some other information.
// I also want to be able to control it with a button or two, and maybe even have it do some simple animations.
// I'm still in the early stages of development, but I'm excited to see where this project goes!



#define TOUCH_PIN 5
#define SCL_PIN 9
#define SDA_PIN 8

void setup() {
  pinMode(TOUCH_PIN, INPUT);
  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();
}

void loop() {
  if (digitalRead(TOUCH_PIN) != HIGH){
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(19, 38, "Touch me");
    u8g2.sendBuffer();

  }
  else{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(19, 38, "Touched");
    u8g2.sendBuffer();
  }
  

   
  

}