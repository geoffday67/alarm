#ifndef __OUTPUT__
#define __OUTPUT__

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <time.h>
#include "FS.h"

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

class Output {
public:
    Output();
    virtual ~Output();
    virtual void clear();
    virtual void showText(int x, int y, const char *ptext, Colours colour);
    virtual void showIdleTime(time_t time);
    virtual void showImage(int x, int y, const char *filename);
    virtual void playSound();
    virtual void soundAlarm();
    virtual void drawRectangle(int x, int y, int width, int height, Colours colour);
    virtual void drawImage(int x, int y, int width, int height, Colours colour, const unsigned char* pimage);
    virtual void drawBitmap(int x, int y, const char *pfilename);

private:
    Adafruit_ILI9341* pTFT;
    uint16_t read16(File &f);
    uint32_t read32(File &f);
    void bmpDraw(String filename, int16_t x, int16_t y);
};

#endif
