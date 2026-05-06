#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Encoder.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// This is the code for my desktop buddy, my main goal is to have a small display that can show the time, date, weather, and maybe some other information.
// I also want to be able to control it with a button or two, and maybe even have it do some simple animations.
// I'm still in the early stages of development, but I'm excited to see where this project goes!
// I like this display because it's small, cheap, and has a good resolution for the size. It's also easy to use with the U8g2 library, which has a lot of features and is well-documented.

// Menu bitmaps:

// [BEGIN lopaka generated]
static const unsigned char image_clock_alarm_bits[] U8X8_PROGMEM = {0x9e,0x3c,0xcd,0x59,0xb7,0x76,0x0b,0x68,0x05,0x50,0x82,0x20,0x82,0x20,0x81,0x40,0x83,0x60,0x41,0x40,0x22,0x20,0x12,0x20,0x04,0x10,0x08,0x08,0xb4,0x16,0xc2,0x21};
static const unsigned char image_FaceNormal_bits[] U8X8_PROGMEM = {0x00,0x00,0x00,0x00,0x3c,0x00,0x80,0x07,0x5e,0x00,0xc0,0x0b,0x7e,0x00,0xc0,0x0f,0x7e,0x00,0xc0,0x0f,0x7e,0x00,0xc0,0x0f,0x3c,0x00,0x80,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x02,0x00,0x00,0x08,0x02,0x00,0x00,0x08,0x02,0x00,0x00,0x10,0x01,0x00,0x00,0xe0,0x00,0x00};
static const unsigned char image_music_headphones_bits[] U8X8_PROGMEM = {0xe0,0x03,0x78,0x0f,0x1c,0x1c,0x06,0x30,0x02,0x20,0x01,0x40,0x01,0x40,0x01,0x40,0x06,0x30,0x09,0x48,0x19,0x4c,0x13,0x64,0x22,0x22,0x26,0x32,0x1c,0x1c,0x00,0x00};
static const unsigned char image_stats_bits[] U8X8_PROGMEM = {0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x18,0x61,0x18,0x61,0x18,0x61,0x1b,0x61,0x1b,0x6d,0x1b,0x6d,0x1b,0x6d,0x1b,0x6d,0x1b,0x6d,0x1b,0x01,0x00,0xff,0x3f,0x00,0x00};



#define CLK_PIN 21
#define DT_PIN 20
#define CONFIRM_PIN 0   // Encoder button
#define TOUCH_PIN 5
#define HOME_PIN 6
#define SCL_PIN 9
#define SDA_PIN 8

const char* menuItems[] = {"Music", "Graph", "Timer", "Buddy"};
const int menuSize = 4;
long oldPos = 0;
int menuIndex = 0;
int buttonState = 0;

Encoder myEnc(CLK_PIN, DT_PIN);

void menuScreen(){
  if (digitalRead(HOME_PIN) == HIGH){
    delay(200);
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.drawXBMP(6, 4, 15, 16, image_music_headphones_bits);
    u8g2.drawXBMP(38, 4, 14, 16, image_stats_bits);
    u8g2.drawXBMP(67, 4, 15, 16, image_clock_alarm_bits);
    u8g2.drawXBMP(93, 6, 29, 14, image_FaceNormal_bits);
    u8g2.sendBuffer();
  }    
}


void setup() {
  Serial.begin(115200);
  pinMode(HOME_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(CONFIRM_PIN, INPUT_PULLUP);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();
}

void loop() {
  menuScreen();

  long newPos = myEnc.read() / 2;   // 2 weil der Encoder 2 Schritte pro Klick macht
  if (newPos != oldPos){
    if (newPos > oldPos){
      menuIndex-- % menuSize;
      delay(50);
      if (menuIndex <0){
        menuIndex = menuSize - 1;   // Index wieder auf letzte element setzen
      }
    }else {
      (menuIndex++ + menuSize) % menuSize;
      delay(50);
      if (menuIndex >= menuSize){
        menuIndex = 0;
      }
    }
    oldPos = newPos;
    Serial.println(menuItems[menuIndex]);
  }
  if (digitalRead(CONFIRM_PIN) == LOW){
    delay(200);
    Serial.println("Selected: " + String(menuItems[menuIndex]));
    delay(200);
  }

  if (digitalRead(TOUCH_PIN)==HIGH){
    delay(200);
    u8g2.clearDisplay();
    u8g2.sendBuffer();
  }

}