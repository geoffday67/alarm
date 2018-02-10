#ifndef __OUTPUT__
#define __OUTPUT__

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <time.h>
#include "FS.h"

/*
Display on a grid of 56 pixel units, this makes a nice button size.
=> 5 units across with left and right margin of 20 pixels,
and 4 units vertically with top and bottom margins of 8 pixels.
*/

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define TOP_MARGIN      8
#define BOTTOM_MARGIN   8
#define LEFT_MARGIN     20
#define RIGHT_MARGIN    20
#define GRID_WIDTH      56
#define GRID_HEIGHT     56
#define LEFT_BUTTON_X   8
#define LEFT_BUTTON_Y   176
#define RIGHT_BUTTON_X  256
#define RIGHT_BUTTON_Y  176

enum class Colours: int {
    Black = 0x0000,
    White = 0xFFFF,
    LightGrey = 0xC618,
    DarkGrey = 0x7BEF,
    Orange = 0xFD20,
    Red = 0xF800,
    Green = 0x07E0,
    Blue = 0x001F
};

class classOutput {
public:
    classOutput();
    virtual ~classOutput();
    virtual void clear();
    virtual void showText(int x, int y, const char *ptext, Colours colour);
    virtual void showText(int x, int y, const char *ptext, Colours foreground, Colours background);
    virtual void showIdleTime(time_t time);
    virtual void showImage(int x, int y, const char *filename);
    virtual void playSound();
    virtual void soundAlarm();
    virtual void drawRectangle(int x, int y, int width, int height, Colours colour);
    virtual void fillRectangle(int x, int y, int width, int height, Colours colour);
    virtual void drawImage(int x, int y, int width, int height, Colours colour, const unsigned char* pimage);
    virtual void drawBitmap(int x, int y, const char *pfilename);
    virtual void drawHorzLine(int x, int y, int length, Colours colour);
    virtual void drawVertLine(int x, int y, int length, Colours colour);

private:
    Adafruit_ILI9341* pTFT;
    uint16_t read16(File &f);
    uint32_t read32(File &f);
    void bmpDraw(String filename, int16_t x, int16_t y);
};

extern classOutput Output;

#endif
