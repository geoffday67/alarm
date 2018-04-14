/*
Try this library for anti-aliased fonts: https://github.com/Bodmer/TFT_eSPI
*/

#ifndef __OUTPUT__
#define __OUTPUT__

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_ILI9341.h>
#include <time.h>

/*
Display on a grid of 56 pixel units, this makes a nice button size.
=> 5 units across with left and right margin of 20 pixels,
and 4 units vertically with top and bottom margins of 8 pixels.
*/

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define TOP_MARGIN      8
#define BOTTOM_MARGIN   8
#define LEFT_MARGIN     8
#define RIGHT_MARGIN    8
#define GRID_WIDTH      56
#define GRID_HEIGHT     56
#define LEFT_BUTTON_X   4
#define LEFT_BUTTON_Y   180
#define RIGHT_BUTTON_X  260
#define RIGHT_BUTTON_Y  180
#define SCREEN_CENTRE   -1

enum class Colours: int {
    Black = 0x0000,
    White = 0xFFFF,
    LightGrey = 0xC618,
    DarkGrey = 0x7BEF,
    Orange = 0xFDA0,
    Red = 0xF800,
    Green = 0x07E0,
    Blue = 0x001F,
    Navy = 0x000F,
    DarkGreen = 0x03E0,
    DarkCyan = 0x03EF,
    Maroon = 0x7800,
    Purple = 0x780F,
    Olive = 0x7BE0,
    Cyan = 0x07FF,
    Magenta = 0x781F,
    Yellow = 0xFFE0,
    GreenYellow = 0xB7E0,
    Pink = 0xFC9F
};

class classOutput {
public:
    classOutput();
    virtual ~classOutput();
    virtual void begin();
    virtual void clear();
    virtual void showText(int x, int y, const char *ptext, Colours colour);
    virtual void showCentredText(int left, int top, int width, int height, const char *ptext, Colours colour);
    virtual void showIdleTime(time_t time);
    virtual void showImage(int x, int y, const char *filename);
    virtual void playSound();
    virtual void startAlarm();
    virtual void stopAlarm();
    virtual void drawRectangle(int x, int y, int width, int height, Colours colour);
    virtual void fillRectangle(int x, int y, int width, int height, Colours colour);
    virtual void drawImage(int x, int y, int width, int height, Colours colour, const unsigned char* pimage);
    virtual void drawBitmap(int x, int y, const char *pfilename);
    virtual void drawHorzLine(int x, int y, int length, Colours colour);
    virtual void drawVertLine(int x, int y, int length, Colours colour);

private:
    //Adafruit_ILI9341* pTFT;
    TFT_eSPI* pTFT;
    TFT_eSprite* pTimeSprite;
    uint16_t read16(File &f);
    uint32_t read32(File &f);
    void bmpDraw(String filename, int16_t x, int16_t y);
};

extern classOutput Output;

#endif
