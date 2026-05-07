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



// [BEGIN lopaka generated]
static const unsigned char image_music_next_bits[] U8X8_PROGMEM = {0x00,0x00,0x07,0xf0,0x09,0x90,0x31,0x90,0x41,0x90,0x81,0x91,0x01,0x92,0x01,0x9c,0x01,0x9c,0x01,0x92,0x81,0x91,0x41,0x90,0x31,0x90,0x09,0x90,0x07,0xf0,0x00,0x00};
static const unsigned char image_music_play_button_bits[] U8X8_PROGMEM = {0xe0,0x03,0x18,0x0c,0x04,0x10,0x02,0x20,0x32,0x20,0xd1,0x41,0x11,0x46,0x11,0x58,0x11,0x46,0xd1,0x41,0x32,0x20,0x02,0x20,0x04,0x10,0x18,0x0c,0xe0,0x03,0x00,0x00};
static const unsigned char image_music_previous_bits[] U8X8_PROGMEM = {0x00,0x00,0x0f,0xe0,0x09,0x90,0x09,0x8c,0x09,0x82,0x89,0x81,0x49,0x80,0x39,0x80,0x39,0x80,0x49,0x80,0x89,0x81,0x09,0x82,0x09,0x8c,0x09,0x90,0x0f,0xe0,0x00,0x00};


void drawMusic_Screen(void) {
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.drawXBMP(55, 44, 15, 16, image_music_play_button_bits);

    u8g2.drawXBMP(90, 44, 16, 16, image_music_next_bits);

    u8g2.drawXBMP(19, 44, 16, 16, image_music_previous_bits);

    u8g2.sendBuffer();
}
// [END lopaka generated]



#define CLK_PIN 21
#define DT_PIN 20
#define CONFIRM_PIN 0   // Encoder button
#define TOUCH_PIN 5     // Touch Sensor still no function
#define RNDM_PIN 6      // Touch Sensor still no function
#define SCL_PIN 9
#define SDA_PIN 8


struct MenuItem {
  int x, y, w, h;
  const unsigned char* icon;
};

MenuItem items[] = {
  {  6, 4, 15, 16, image_music_headphones_bits },
  { 38, 4, 14, 16, image_stats_bits            },
  { 67, 4, 15, 16, image_clock_alarm_bits       },
  { 93, 6, 29, 14, image_FaceNormal_bits        },
};

const int menuSize = 4;
long oldPos = 0;
int menuIndex = 0;
int buttonState = 0;

Encoder myEnc(CLK_PIN, DT_PIN);

void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  for (int i = 0; i < menuSize; i++) {
    MenuItem& m = items[i];
    int pad = 4;
    int bx = m.x - pad;
    int by = m.y - pad;
    int bw = m.w + pad * 2;
    int bh = m.h + pad * 2;

    if (i == menuIndex) {
      // weißes Rechteck + Icon invertiert (schwarz auf weiß)
      u8g2.setDrawColor(1);
      u8g2.drawRBox(bx, by, bw, bh, 3);
      u8g2.setDrawColor(2);   // XOR → invertiert das Icon
      u8g2.drawXBMP(m.x, m.y, m.w, m.h, m.icon);
    } else {
      // nur Outline + normales Icon
      u8g2.setDrawColor(1);
      u8g2.drawRFrame(bx, by, bw, bh, 3);
      u8g2.drawXBMP(m.x, m.y, m.w, m.h, m.icon);
    }
  }

  u8g2.setDrawColor(1);  // zurücksetzen für nächste draws
  u8g2.sendBuffer();
}

void onSelect(int i) {
  u8g2.clearDisplay();
  if(menuIndex == 0){
    drawMusic_Screen();
    if (digitalRead(TOUCH_PIN) == HIGH){
      // TODO: NEXT, PREVIOS, PLAY/PAUSE functions
    }
  }
  if(menuIndex == 1){
    
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(RNDM_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(CONFIRM_PIN, INPUT_PULLUP);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();
  drawMenu();
}

void loop() {

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
    drawMenu();
  }
  if (digitalRead(CONFIRM_PIN) == LOW){
    onSelect(menuIndex);
    delay(200);
  }

  if (digitalRead(TOUCH_PIN)==HIGH){
    delay(200);
    u8g2.clearDisplay();
    u8g2.sendBuffer();
  }

}