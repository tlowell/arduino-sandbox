/*
  waveshare-esp32-s3-lcd-1.47-hello-world.ino
  Source: github.com/tlowell/arduino-sandbox
 
  Copyright (c) 2026 Thomas Lowell
  Licensed under the MIT License.
 
  For the Waveshare ESP32-S3-TOUCH-LCD-1.47 board only due to specific pin usage.

  This sketch is for the ESP32-S3-TOUCH-LCD-1.47. Volos' was for the ESP32-S3-TOUCH-LCD-2. They
  are different boards with different displays and required these modifications to his script:

    1. The ESP32-S3-TOUCH-LCD-1.47 has a different set of LCD and TFT pins for the display.
    2. This board has an IPS display (not capacitive touch) requiring a different touch library.
    3. This display is 320x173, not 320x240, requiring different math for the squares spacing.
    4. The color literals (BLACK, GREEN, etc.) resulted in compile time errors. To avoid this, 
       Adafruit_ST7789.h is includeded and the ST77XX_xxx color palette is used. 

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

// screen orientation
#define LCD_PORTRAIT_USB_BOTTOM 4
#define LCD_LANDSCAPE_USB_LEFT 5
#define LCD_PORTRAIT_USB_TOP 6
#define LCD_LANDSCAPE_USB_RIGHT 7
#define LCD_ROTATION LCD_PORTRAIT_USB_BOTTOM

#define LCD_TEXT_SIZE 1
#define LCD_H_RES 172 // portrait mode. so we flip these when in landscape mode
#define LCD_V_RES 320 // ESP32-S3-TOUCH-LCD-1.47 display is 320 x 172

// LCD pins for this board (see schematic on Waveshare wiki)
#define LCD_BL 46 // verified 
#define LCD_CS 21
#define LCD_SCL 38  // On these boards: LCD_SCL = SPI SCK
#define LCD_SDA 39  // On these boards: LCD_SDA = SPI MOSI
#define LCD_MISO -1 // On these boards: There is no MISO listed → this display is write-only (very common for ST7789)
#define LCD_RST 40
#define LCD_DC 45
#define LCD_BL 46

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCL, LCD_SDA, LCD_MISO); 
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
}
