#include "Screen.h"
#include "Output.h"

Screen::Screen () {
}

Screen::~Screen() {}

void Screen::showTitle(const char* ptext) {
    Output.fillRectangle(0, 0, SCREEN_WIDTH, 40, Colours::Green);
    int x = (SCREEN_WIDTH - strlen(ptext) * 2) / 2;
    Output.showText(x, 16, ptext, Colours::White);
}
