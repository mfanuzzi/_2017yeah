// 2017! 2017! 2017!
// Scroll that across the display! At a 45 degree angle! YES!

#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_DotStarMatrix_MF.h"
#include <Adafruit_DotStar.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#include <Fonts/FreeSerif9pt7b.h>

#define DATAPIN  4
#define CLOCKPIN 5
#define RESETPIN 2 
#define DELAYPIN 12 // Pin to indicate this is the 2nd arduino of the pair and delay the animation

#define WIDTH 13
#define HEIGHT 13

// BEGIN EASY SETTINGS

#define TEXTMODE false  // If true, uses the specified font. If false, uses the binary image data
#define FLIPPED true    // If true, that means the cord is comin out of the bottom
#define DELAYAMOUNT 23  // Number of "frames" to delay the 2nd board by
#define FRAMEDURATION 70 // In ms
#define BRIGHTNESS 70   // Out of 255. Probably want to crank it up for the shot, but otherwise that's blinding.

// END EASY SETTINGS :)

#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif

 // Images. Converted from 2017.png with:
 // http://javl.github.io/image2cpp/
 // SIZES MUST BE MULTIPLES OF 8! I am using 16h x 56w.
 // '2017'
/*const unsigned char myBitmap [] PROGMEM = {
  0x0e, 0x03, 0x80, 0xc3, 0xfe, 0x00, 0x00, 0x1f, 0x07, 0xe7, 0xc3, 0xfe, 0x00, 0x00, 0x33, 0x8c, 
  0x64, 0xc0, 0x04, 0x00, 0x00, 0x21, 0x8c, 0x60, 0xc0, 0x0c, 0x00, 0x00, 0x01, 0x98, 0x30, 0xc0, 
  0x0c, 0x00, 0x00, 0x01, 0x98, 0x30, 0xc0, 0x08, 0x00, 0x00, 0x01, 0x18, 0x30, 0xc0, 0x18, 0x00, 
  0x00, 0x03, 0x18, 0x30, 0xc0, 0x18, 0x00, 0x00, 0x06, 0x18, 0x30, 0xc0, 0x10, 0x00, 0x00, 0x06, 
  0x18, 0x30, 0xc0, 0x30, 0x00, 0x00, 0x0c, 0x18, 0x30, 0xc0, 0x30, 0x00, 0x00, 0x18, 0x18, 0x30, 
  0xc0, 0x60, 0x00, 0x00, 0x30, 0x0c, 0x60, 0xc0, 0x60, 0x00, 0x00, 0x20, 0x0c, 0x60, 0xc0, 0x60, 
  0x00, 0x00, 0x7f, 0x87, 0xc7, 0xf8, 0xc0, 0x00, 0x00, 0x7f, 0x83, 0x87, 0xf8, 0xc0, 0x00, 0x00
};*/
 // '2017-sm'
/*const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x89, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x96, 0xc8, 0x10, 0x00, 0x00, 0x00, 0x00, 0x14, 0x48, 0x20, 0x00, 0x00, 0x00, 
  0x00, 0x14, 0x48, 0x20, 0x00, 0x00, 0x00, 0x00, 0x24, 0x48, 0x40, 0x00, 0x00, 0x00, 0x00, 0x44, 
  0x48, 0x40, 0x00, 0x00, 0x00, 0x00, 0x86, 0xc8, 0x80, 0x00, 0x00, 0x00, 0x00, 0xf3, 0x9c, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};*/
// with 0-cross
 // '2017-sm'
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x89, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x96, 0xc8, 0x10, 0x00, 0x00, 0x00, 0x00, 0x14, 0x48, 0x20, 0x00, 0x00, 0x00, 
  0x00, 0x14, 0xc8, 0x20, 0x00, 0x00, 0x00, 0x00, 0x25, 0x48, 0x40, 0x00, 0x00, 0x00, 0x00, 0x46, 
  0x48, 0x40, 0x00, 0x00, 0x00, 0x00, 0x86, 0xc8, 0x80, 0x00, 0x00, 0x00, 0x00, 0xf3, 0x9c, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


float matrixrot  = (PI / 4); // = 45 deg in rad
float cosrot = cos(matrixrot); // precalculate these for better performance. it's an arduino.
float sinrot = sin(matrixrot);

int16_t vWIDTH = WIDTH + 7; // "virtual" widths and heights for dealing with the
int16_t vHEIGHT = HEIGHT + 7; //  extra off-canvas space needed for a non-90 deg rotation

// MATRIX DECLARATION:
// Parameter 1 = width of DotStar matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   DS_MATRIX_TOP, DS_MATRIX_BOTTOM, DS_MATRIX_LEFT, DS_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     DS_MATRIX_TOP + DS_MATRIX_LEFT for the top-left corner.
//   DS_MATRIX_ROWS, DS_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   DS_MATRIX_PROGRESSIVE, DS_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type:
//   DOTSTAR_BRG  Pixels are wired for BRG bitstream (most DotStar items)
//   DOTSTAR_GBR  Pixels are wired for GBR bitstream (some older DotStars)

Adafruit_DotStarMatrix_MF matrix = Adafruit_DotStarMatrix_MF(
  vWIDTH, vHEIGHT, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_RIGHT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_BRG);
//Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
//  8, 8,
//  DS_MATRIX_TOP     + DS_MATRIX_RIGHT +
//  DS_MATRIX_COLUMNS + DS_MATRIX_PROGRESSIVE,
//  DOTSTAR_BRG);

// SET COLOR HERE
const uint16_t colors[] = {
  matrix.Color(30, 153, 255) // The color to display the text/image in. Going with a vaguely cerulian blue.
};


/*
void myRemapFn2(int16_t &x, int16_t &y) {

  int16_t x1 = (x * cosrot) - (y * cosrot);
  int16_t y1 = (x * sinrot) + (y * cosrot); 

//  y1 -= 7;
//  x1 += 5;

  x = x1;
  y = y1;
}
*/
uint16_t myRemapFn(uint16_t x, uint16_t y) {
    uint16_t minor, major, majorScale, pixelOffset;

    int16_t x1 = (x * cosrot) - (y * cosrot);
    int16_t y1 = (x * sinrot) + (y * cosrot); 

    if (TEXTMODE) {
      // For using text...
      y1 -= 11;
      x1 += 2;      
    }
    else {
      // For using images
      y1 -= 5;
      x1 += 6;
    }
    
    // Find pixel number within tile
    minor = x1; // Presume row major to start (will swap later if needed)
    major = y1;

    // By default we are TOP RIGHT
    // If we want to flip this we need BOTTOM LEFT

    if (FLIPPED) {
    major = HEIGHT - 1 - major;
    } else {
    minor = WIDTH  - 1 - minor;
    }
    majorScale = WIDTH;

    // because columns
    _swap_uint16_t(major, minor);
    majorScale = HEIGHT;

    // Zigzag
    if(major & 1) pixelOffset = (major + 1) * majorScale - 1 - minor;
    else          pixelOffset =  major      * majorScale     + minor;

    if ((x1 > WIDTH) || (x1 < 0) || (y1 > HEIGHT) || (y1 < 0))
      return 666;
      
    return pixelOffset;
}

int IsDelayed = 0;
int delaycount = 0;

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.setFont(&FreeSerif9pt7b);
  matrix.setTextColor(colors[0]);
  matrix.setTextSize(1);
  matrix.setRemapFunction(myRemapFn);
  //matrix.setRemapFunction2(myRemapFn2);

  pinMode(RESETPIN, INPUT);
  digitalWrite(RESETPIN, HIGH);

  pinMode(DELAYPIN, INPUT);
  digitalWrite(DELAYPIN, HIGH);

  delay(10);
  IsDelayed = !digitalRead(DELAYPIN);
  if (IsDelayed) {
    delaycount = DELAYAMOUNT;
  }
}

int x    = matrix.width();
int pass = 0;

void loop() {
  int reset = digitalRead(RESETPIN);
  if (reset) {
    x = matrix.width();
    pass = 0;
    if (IsDelayed) {
      delaycount = DELAYAMOUNT;
    }
  }

  if (delaycount > 0)
  {
    --delaycount;
  }
  else {
    if (TEXTMODE) {
      loopText();
    } else {
      loopImg();    
    }
  }
  delay(FRAMEDURATION);
}

void loopText() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 15);
  matrix.print("2017");
  if(--x < -36) {
    x = matrix.width();
    //if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[0]);
  }
  matrix.show();
}

void loopImg() {
  matrix.fillScreen(0);
  matrix.drawBitmap(x, 0, myBitmap, 56, 16, colors[0]);
  if(--x < -35) { //Steps to scroll before repeating. Use 56, if doing the full thing.
    x = matrix.width();
    //if(++pass >= 3) pass = 0;
  }
  matrix.show();
}
