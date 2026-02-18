/*
  waveshare-esp32-s3-lcd-1.47-hello-world.ino
  Source: github.com/tlowell/arduino-sandbox
 
  Copyright (c) 2026 Thomas Lowell
  Licensed under the MIT License.
 
  For the Waveshare ESP32-S3-LCD-1.47 board only due to specific pin usage.

  Arduino IDE settings:
    Board: ESP32S3 Dev Module
  	Under Arduino Tools also set…
		  Flash size -> 16 MB (128 Mb)
		  Partitian -> 16 MB (3MB…
		  PSRAM -> OPI...
*/

#include <Arduino_GFX_Library.h>
#include <Adafruit_ST7789.h>  

// screen orientation
#define LCD_PORTRAIT_USB_BOTTOM 0 // set the 3rd bit (or add 4) to these values to reverse the text 
#define LCD_LANDSCAPE_USB_RIGHT 1
#define LCD_PORTRAIT_USB_TOP 2
#define LCD_LANDSCAPE_USB_LEFT 3
#define LCD_ROTATION LCD_PORTRAIT_USB_BOTTOM 

#define LCD_TEXT_SIZE 2
#define LCD_H_RES 240 
#define LCD_V_RES 320

// LCD pins for this board (see schematic on Waveshare wiki)
#define LCD_BL  46
#define LCD_CS  42
#define LCD_SCK 40
#define LCD_MISO -1
#define LCD_DIN 45
#define LCD_RST 39
#define LCD_DC 41
#define LCD_BL 46 
	
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_DIN, LCD_MISO); 
Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST, LCD_ROTATION, true /* IPS */, LCD_H_RES, LCD_V_RES);

 void setup() {
  Serial.begin(115200);
  gfx->begin();
  gfx->setRotation(LCD_ROTATION); // 0 is scross the narrow, reversed. 1 is across the wide, reversed, 3 is acorss wide other side. reeverses/
  gfx->displayOn();

  gfx->fillScreen(ST77XX_BLACK);
  gfx->setTextSize(LCD_TEXT_SIZE);
  gfx->setCursor(50,50);
  gfx->setTextColor(ST77XX_GREEN);

  gfx->print("Hello World!");
  analogWrite(LCD_BL, 150);
}

void loop() {
}
