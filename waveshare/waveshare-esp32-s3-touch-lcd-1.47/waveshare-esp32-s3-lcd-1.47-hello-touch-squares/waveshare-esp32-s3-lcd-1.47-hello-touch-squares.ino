/*
  waveshare-esp32-s3-lcd-1.47-hello-touch-squares.ino
  Source: github.com/tlowell/arduino-sandbox
 
  Copyright (c) 2026 Thomas Lowell
  Licensed under the MIT License.
 
  For the Waveshare ESP32-S3-TOUCH-LCD-1.47 board only due to specific pin usage.

  This sketch emulates the sketch written by YouTuber Volos that he wrote in this video: https://www.youtube.com/watch?v=oPAOMTf5dVE

  This sketch is for the ESP32-S3-TOUCH-LCD-1.47. Volos' was for the ESP32-S3-TOUCH-LCD-2. They
  are different boards with different displays and required these modifications to his script:

    1. The ESP32-S3-TOUCH-LCD-1.47 has a different set of LCD and TFT pins for the display.
    2. This board has an IPS display (not capacitive touch) requiring a different touch library.
    3. This display is 320x173, not 320x240, requiring different math for the squares spacing.
    4. The color literals (BLACK, GREEN, etc.) resulted in compile time errors. To avoid this, 
       Adafruit_ST7789.h is includeded and the ST77XX_xxx color palette is used. 

  Additionally, I added two helper functions so the orientation of the board can be changed from
  portrait to landscape.

    gfxPrintFourCorners - writes the display coordinates to all four corners of the display so the
                          mapping of screen orientation to coordinates can be easily debugged.
    transformTouchCoordinates - maps the x , y   touch coordinates to new values based on screen orientation.              

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
#include <lvgl.h>
#include <esp_lcd_touch_axs5106l.h> // this board is IPS, not cap touch so no bb_captouch library.

void gfxPrintFourCorners(int width, int height, int rotation_setting, int char_height, int char_width, int margin);
void transformTouchCoordinates(int &x, int &y);

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

//Touch Pins for this board (see schematic on Waveshare wiki)
#define TP_SDA 42
#define TP_SCL 41
#define TP_RST 47
#define TP_INT 48

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCL, LCD_SDA, LCD_MISO); 
Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST, LCD_ROTATION, false /* IPS */, LCD_H_RES, LCD_V_RES, 34, 0, 34, 0);

/* for drawing in four corners  */
#define CHAR_HEIGHT LCD_TEXT_SIZE * 5
#define CHAR_WIDTH LCD_TEXT_SIZE * 6
#define MARGIN 10 // boarder around display to avoid clipping
//
//Volos's 3 colored boxes from his you tube video (How to Program an ESP32 DEv Board (Part 2) - Touch Inputs Tutorial)
int box_size = 80;
int xcor = (LCD_H_RES - box_size) / 2; // Volos used 80 (1/3 of horizonal dimension. because his display is 240. Mine is 172. Keeping box 80 we have (172-80)/2 = 46
int vertical_spacing  = 20; 
int ycor1 = vertical_spacing; // 20
int ycor2 = vertical_spacing + box_size + vertical_spacing; // 120
int ycor3 = vertical_spacing + box_size + vertical_spacing + box_size + vertical_spacing; // 220
 
 void setup() {
  //lcd reset
  pinMode(LCD_RST, OUTPUT);
  digitalWrite(LCD_RST,0);
  delay(10);
  digitalWrite(LCD_RST,1);

  Serial.begin(115200);
  delay(100);
  Serial.println("Hello Touch Squares example");

  gfx->begin();
  gfx->setRotation(LCD_ROTATION); // 0 is scross the narrow, reversed. 1 is across the wide, reversed, 3 is acorss wide other side. reeverses/
  gfx->displayOn();
  gfx->fillScreen(ST77XX_BLACK);
  //
  // There's a bug where the red and blue squares are transposed dispite the touch coordinates looking
  // correct when displayed with gfx in gfxPrintFourCorners. Haven't tracked this bug down
  // yet. Cheat and swpa them here for drawing. The touch input in loop() will be correct.
  //
  gfx->fillRect(xcor, ycor3, box_size, box_size, ST77XX_RED);//bug: ycor1 should work
  gfx->fillRect(xcor, ycor2, box_size, box_size, ST77XX_GREEN);
  gfx->fillRect(xcor, ycor1, box_size, box_size, ST77XX_BLUE); //bug: ycor3 should work

  Serial.println("red =" + String(ycor1) + ", expected 20");
  Serial.println("green =" + String(ycor2) + ", expected 120");
  Serial.println("blue =" + String(ycor3) + ", expected 220");

  // Uncomment this call for help debugging screen coordinates
  gfxPrintFourCorners(LCD_H_RES, LCD_V_RES, LCD_ROTATION, CHAR_HEIGHT, CHAR_WIDTH, MARGIN);

  analogWrite(LCD_BL, 150);
  Wire.begin(TP_SDA, TP_SCL);
  bsp_touch_init(&Wire, TP_RST, TP_INT, gfx->getRotation(), gfx->width(), gfx->height());
  lv_init();
}

void loop() {
  bool touchpad_pressed = false;
  touch_data_t touch_data;
  static int x = 0, y = 0;

   bsp_touch_read();
   touchpad_pressed = bsp_touch_get_coordinates(&touch_data);
    if (touchpad_pressed && (x != touch_data.coords[0].x || y != touch_data.coords[0].y)) { 
      // one of the two coordinates changed so do something...
      x = touch_data.coords[0].x;
      y = touch_data.coords[0].y;
      gfx->fillRect(15,20, 30,20, ST77XX_BLACK );
      gfx->setCursor(15,30);
      // Serial.println("\n" + String(x) + "," + String(y)); For debugging orientation. Check coordinates.
      transformTouchCoordinates(x, y); // map touch coords to current display orientation
      // Serial.println(String(x) + "," + String(y)); // For debugging orientation. Did we change then for this orientation?
      /*
          Check if touch cordinates are on a colored box.
      */
      if (x>= xcor && x <= xcor + box_size){
        // we * may * have touched a box. Gotta check y
        if (y >= ycor1 && y <= ycor1 + box_size){ // red
           gfx->print("RED");
        } else if (y >= ycor2 && y <= ycor2 + box_size) { // green
           gfx->print("GREEN");
        }  else if (y >= ycor3 && y <= ycor3 + box_size) {// blue 
           gfx->print("BLUE");
        }
      }
    // Uncomment this call for help debugging screen coordinates 
    gfx->print(String(x) + ", " + String(y));
    }
}

void transformTouchCoordinates(int &x, int &y)
{
    if (LCD_ROTATION == LCD_PORTRAIT_USB_TOP) {
        x = LCD_H_RES + x;
        y = LCD_V_RES - y;
    } else if (LCD_ROTATION == LCD_PORTRAIT_USB_BOTTOM) {

        // currently no transform

    } else if (LCD_ROTATION == LCD_LANDSCAPE_USB_LEFT) {
        int temp = x;
        x = LCD_V_RES - y;
        y = temp - LCD_H_RES + 20;
    } else if (LCD_ROTATION == LCD_LANDSCAPE_USB_RIGHT) {
        int temp = x;
        x = y;
        y = (2 * LCD_H_RES - temp - 30);
    }
}

void gfxPrintFourCorners(int width, int height, int rotation_setting, int char_height, int char_width, int margin){
  /*
     Test function that prints display coordinate in all four corners.  Origin (0,0) assumed to be upper left
     The purpose is to visually verify assumptions match what the code is seeing.

     - example:   0,0    (UL)  240,0   (UR)  
                  0, 320 (LL)  240,320 (LR)

     - width and height are the physical display dimensions (but could be smaller)
     - margin is the boarder inside the physical display to not be drawn on to avoid clipping.
     - char_height and char_width are to accomodate the size of the string we're printing so it doesn't run off.
  */
int string_length, n_chars, temp;
if (rotation_setting == LCD_LANDSCAPE_USB_LEFT){ // landscape mode 
  temp = width;
  width = height;
  height = temp;
}
n_chars = 13; // eg "150, 120 (LL)"
string_length = char_width * n_chars;
if (Serial) Serial.println("rotation = " + String(rotation_setting));
// don't don't display the margin or string_length ub effect. drop it from the print math. we really want to see the screen size being used. 

// upper left  (0, 0) 
gfx->setCursor(0 + margin , 0 + margin); //  (0, 0). upper left.
gfx->print(String(0) + "," + String(0) +  " (UL)"); 
if (Serial) Serial.print(String(0) + "," + String(0) +  "   (UL)     ");

// upper right (width, 0)
gfx->setCursor(width - string_length - margin , 0 + margin); // upper right
gfx->print(String(width) + "," + String(0) +  " (UR)"); 
if (Serial) Serial.println(String(width) + "," + String(0) +  "   (UR)"); 

// lower left (0, height)
gfx->setCursor(0 + margin, height - char_height - margin); // lower left
gfx->print(String(0) + "," + String( height) +  " (LL)"); 
if (Serial) Serial.print(String(0) + "," + String( height) +  " (LL)     "); 

 // lower right (width, height)
gfx->setCursor(width - string_length - margin, height - char_height - margin); // lower right
gfx->print(String(width) + "," + String(height) +  " (LR)"); 
if (Serial) Serial.println(String(width) + "," + String(height) +  " (LR)"); 
}
