/*
  waveshare-esp32-s3-lcd-1.47-hello-world.ino
  Source: github.com/tlowell/arduino-sandbox
 
  Copyright (c) 2026 Thomas Lowell
  Licensed under the MIT License.
 
  For the Waveshare ESP32-S3-TOUCH-LCD-1.47 board only due to specific pin usage.

  Arduino IDE settings:
    Board: ESP32S2 Dev Module
  	Under Arduino Tools also set…
		  USB CDC On Booot "enabled"
		  Flash size -> 16 MB
		  Partitian -> 16 MB (3MB…
		  PSRAM -> OPI

*/

#include <Arduino_GFX_Library.h>
#include <Adafruit_ST7789.h>  

#define LCD_ROTATION 5 // 4 for across the narrow dimension, 5 for across the wide dimension
#define LCD_TEXT_SIZE 2
#define LCD_H_RES 240 // correct oritentation for this board.
#define LCD_V_RES 320
#define LCD_BL 46 // verified 

#define LCD_CS 21
#define LCD_SCL 38
#define LCD_SDA 39
#define LCD_RST 40
#define LCD_DC 45
#define LCD_BL 46
#define LCD_MISO -1

// On these boards:
//	•	LCD_SCL = SPI SCK
//	•	LCD_SDA = SPI MOSI
// There is no MISO listed → this display is write-only (very common for ST7789)

//Arduino_DataBus *bus = new Arduino_ESP32SPI(42 /* DC */, 45 /* CS */, 39 /* SCK */, 38 /* MOSI */, 40 /* MISO */);
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCL, LCD_SDA, LCD_MISO); 
//Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST, LCD_ROTATION, true /* IPS */, LCD_H_RES, LCD_V_RES);
Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST, LCD_ROTATION, false /* IPS */, LCD_H_RES, LCD_V_RES);

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
  // put your main code here, to run repeatedly:
 // Serial.println("Hello World!");
//delay(5000);
}
