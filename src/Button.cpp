#include "Button.h"
#include "EventManager.h"
#include "Event.h"
#include "Output.h"

Button::Button(int left, int top) {
    this->left = left;
    this->top = top;
    EventManager.addListener(EVENT_TOUCH, this);
}

Button::Button(int left, int top, int width, int height) {
    id = 0;
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;
    EventManager.addListener(EVENT_TOUCH, this);
    showUp();
}

Button::~Button() {
    EventManager.removeListener(this);
}

bool Button::contains(int x, int y)
{
    return (x >= left && x < left + width && y > top && y < top + height);
}

void Button::showDown()
{
    Output.drawRectangle(left, top, width, height, Colours::Blue);
    Output.drawRectangle(left + 1, top + 1, width - 2, height - 2, Colours::Blue);
    //Output.drawRectangle(left + 2, top + 2, width - 4, height - 4, Colours::Blue);
}

void Button::showUp()
{
    Output.drawRectangle(left, top, width, height, Colours::DarkGrey);
    Output.drawRectangle(left + 1, top + 1, width - 2, height - 2, Colours::DarkGrey);
    //Output.drawRectangle(left + 2, top + 2, width - 4, height - 4, Colours::DarkGrey);
}

void Button::onEvent(Event* pevent) {
    TouchEvent *ptouch = (TouchEvent*) pevent;
    if (contains (ptouch->x, ptouch->y)) {
        if (ptouch->down) {
            showDown();
        } else {
            showUp();
            EventManager.queueEvent(new ButtonEvent(id));
        }        
    }
}

/*
Create byte array using http://javl.github.io/image2cpp/ and https://material.io/icons/
Download 24dp black and use web 2x.
Choose background colour black, and invert image colours.
Centre in 56 x 56 canvas.
*/

const unsigned char bitmapSave [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 
	0xc0, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3f, 0xf0, 0x00, 
	0x00, 0x3c, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3c, 
	0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x3f, 0xfc, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 
	0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
	0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 
	0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 
	0xfc, 0x00, 0x00, 0x3f, 0xff, 0xc3, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0x00, 0xff, 0xfc, 0x00, 
	0x00, 0x3f, 0xfe, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x3f, 
	0xfc, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 
	0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x7f, 0xfc, 
	0x00, 0x00, 0x3f, 0xfe, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0x00, 0xff, 0xfc, 0x00, 0x00, 
	0x3f, 0xff, 0xc3, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 
	0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0
};

const unsigned char bitmapBack [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xff, 0xff, 
	0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x3f, 
	0xff, 0x7f, 0xfe, 0xff, 0xf0, 0x00, 0x3f, 0xfe, 0x3f, 0xfc, 0x7f, 0xf0, 0x00, 0x7f, 0xfc, 0x1f, 
	0xf8, 0x3f, 0xf0, 0x00, 0xff, 0xfe, 0x0f, 0xf0, 0x7f, 0xf0, 0x00, 0xff, 0xff, 0x07, 0xe0, 0xff, 
	0xf0, 0x01, 0xff, 0xff, 0x83, 0xc1, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xc1, 0x83, 0xff, 0xf0, 0x03, 
	0xff, 0xff, 0xe0, 0x07, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 
	0xf8, 0x1f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xf0, 0x0f, 
	0xff, 0xf0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xc1, 0x83, 0xff, 0xf0, 
	0x01, 0xff, 0xff, 0x83, 0xc1, 0xff, 0xf0, 0x00, 0xff, 0xff, 0x07, 0xe0, 0xff, 0xf0, 0x00, 0xff, 
	0xfe, 0x0f, 0xf0, 0x7f, 0xf0, 0x00, 0x7f, 0xfc, 0x1f, 0xf8, 0x3f, 0xf0, 0x00, 0x3f, 0xfe, 0x3f, 
	0xfc, 0x7f, 0xf0, 0x00, 0x3f, 0xff, 0x7f, 0xfe, 0xff, 0xf0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
	0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 
	0xff, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xff, 
	0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0
};

const unsigned char bitmapSettings [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x60, 0x7f, 0xfe, 0x06, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
	0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xe0, 0x00, 
	0x00, 0x07, 0xff, 0xf0, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 
	0x00, 0x01, 0xff, 0xc0, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0x80, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x01, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0x80, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x01, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x03, 0xff, 0x80, 0x00, 
	0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x0f, 0xff, 0xe0, 0x00, 
	0x00, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
	0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0xfe, 0x06, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapChecked [] PROGMEM = {
	// 'ic_radio_button_checked_black_24dp_2x, 56x56px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x7f, 
	0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xff, 0x80, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x1f, 
	0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x03, 0xf0, 0x00, 
	0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x3e, 
	0x00, 0x7e, 0x00, 0x7c, 0x00, 0x00, 0x3c, 0x01, 0xff, 0x80, 0x3c, 0x00, 0x00, 0x7c, 0x07, 0xff, 
	0xe0, 0x3e, 0x00, 0x00, 0x78, 0x0f, 0xff, 0xf0, 0x1e, 0x00, 0x00, 0x78, 0x0f, 0xff, 0xf0, 0x1e, 
	0x00, 0x00, 0x78, 0x1f, 0xff, 0xf8, 0x1e, 0x00, 0x00, 0xf0, 0x1f, 0xff, 0xf8, 0x0f, 0x00, 0x00, 
	0xf0, 0x3f, 0xff, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x3f, 0xff, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x3f, 
	0xff, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x3f, 0xff, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x3f, 0xff, 0xfc, 
	0x0f, 0x00, 0x00, 0xf0, 0x3f, 0xff, 0xfc, 0x0f, 0x00, 0x00, 0xf0, 0x1f, 0xff, 0xf8, 0x0f, 0x00, 
	0x00, 0x78, 0x1f, 0xff, 0xf8, 0x1e, 0x00, 0x00, 0x78, 0x0f, 0xff, 0xf0, 0x1e, 0x00, 0x00, 0x78, 
	0x0f, 0xff, 0xf0, 0x1e, 0x00, 0x00, 0x7c, 0x07, 0xff, 0xe0, 0x3e, 0x00, 0x00, 0x3c, 0x01, 0xff, 
	0x80, 0x3c, 0x00, 0x00, 0x3e, 0x00, 0x7e, 0x00, 0x7c, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xf8, 
	0x00, 0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x00, 
	0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x01, 0xff, 
	0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapUnchecked [] PROGMEM = {
	// 'ic_radio_button_unchecked_black_24dp_2x, 56x56px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x7f, 
	0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xff, 0x80, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x1f, 
	0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x03, 0xf0, 0x00, 
	0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x3e, 
	0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7c, 0x00, 0x00, 
	0x00, 0x3e, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 
	0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 
	0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x00, 
	0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
	0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 
	0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x78, 
	0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x3c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xf8, 
	0x00, 0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x00, 
	0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x01, 0xff, 
	0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapPower [] PROGMEM = {
	// 'ic_power_settings_new_black_24dp_2x, 56x56px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x3c, 0x03, 0x80, 0x00, 
	0x00, 0x03, 0xe0, 0x3c, 0x07, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x3c, 0x07, 0xe0, 0x00, 0x00, 0x07, 
	0xc0, 0x3c, 0x03, 0xe0, 0x00, 0x00, 0x0f, 0x80, 0x3c, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x3c, 
	0x00, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0x3c, 0x00, 0xf8, 0x00, 0x00, 0x1e, 0x00, 0x3c, 0x00, 0x78, 
	0x00, 0x00, 0x1e, 0x00, 0x3c, 0x00, 0x78, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 
	0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
	0x3c, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 
	0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 
	0x00, 0x1e, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x1f, 
	0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 
	0x01, 0xf0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 
	0x00, 0x00, 0x03, 0xf0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x3f, 0x80, 0x00, 0x00, 
	0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x1f, 
	0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapAlarm [] PROGMEM = {
	// 'ic_alarm_black_24dp_2x, 56x56px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x01, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x07, 0xe0, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x1f, 0xc0, 0xff, 0x03, 0xf8, 0x00, 0x00, 0x3f, 0x07, 0xff, 0xe1, 
	0xfc, 0x00, 0x00, 0x7e, 0x1f, 0xff, 0xf8, 0x7e, 0x00, 0x00, 0xfc, 0x7f, 0xff, 0xfe, 0x3f, 0x00, 
	0x00, 0x78, 0xff, 0x00, 0xff, 0x1e, 0x00, 0x00, 0x31, 0xfc, 0x00, 0x3f, 0x8c, 0x00, 0x00, 0x03, 
	0xf0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x38, 
	0x03, 0xe0, 0x00, 0x00, 0x0f, 0x80, 0x38, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x38, 0x00, 0xf0, 
	0x00, 0x00, 0x1f, 0x00, 0x38, 0x00, 0xf8, 0x00, 0x00, 0x1e, 0x00, 0x38, 0x00, 0x78, 0x00, 0x00, 
	0x1e, 0x00, 0x38, 0x00, 0x78, 0x00, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
	0x38, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x38, 0x00, 
	0x3c, 0x00, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x3e, 0x00, 0x3c, 0x00, 
	0x00, 0x3c, 0x00, 0x1f, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0xc0, 0x3c, 0x00, 0x00, 0x1e, 
	0x00, 0x03, 0xf0, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x00, 0xf8, 0x78, 0x00, 0x00, 0x1f, 0x00, 0x00, 
	0x70, 0xf8, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 
	0x00, 0x00, 0x07, 0xc0, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 
	0x03, 0xf0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0xff, 
	0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapSnooze [] PROGMEM = {
	// 'ic_snooze_black_24dp_2x, 56x56px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x01, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x07, 0xe0, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x1f, 0xc0, 0xff, 0x03, 0xf8, 0x00, 0x00, 0x3f, 0x07, 0xff, 0xe1, 
	0xfc, 0x00, 0x00, 0x7e, 0x1f, 0xff, 0xf8, 0x7e, 0x00, 0x00, 0xfc, 0x7f, 0xff, 0xfe, 0x3f, 0x00, 
	0x00, 0x78, 0xff, 0x00, 0xff, 0x1e, 0x00, 0x00, 0x31, 0xfc, 0x00, 0x3f, 0x8c, 0x00, 0x00, 0x03, 
	0xf0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x00, 
	0x03, 0xe0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x03, 0xff, 0xc0, 0xf0, 
	0x00, 0x00, 0x1f, 0x03, 0xff, 0xc0, 0xf8, 0x00, 0x00, 0x1e, 0x03, 0xff, 0xc0, 0x78, 0x00, 0x00, 
	0x1e, 0x03, 0xff, 0xc0, 0x78, 0x00, 0x00, 0x3c, 0x00, 0x07, 0x80, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
	0x0f, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x1e, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x3e, 0x00, 
	0x3c, 0x00, 0x00, 0x3c, 0x00, 0x7c, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0xf8, 0x00, 0x3c, 0x00, 
	0x00, 0x3c, 0x00, 0xf0, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x01, 0xe0, 0x00, 0x3c, 0x00, 0x00, 0x1e, 
	0x03, 0xff, 0xc0, 0x78, 0x00, 0x00, 0x1e, 0x03, 0xff, 0xc0, 0x78, 0x00, 0x00, 0x1f, 0x03, 0xff, 
	0xc0, 0xf8, 0x00, 0x00, 0x0f, 0x03, 0xff, 0xc0, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x01, 0xf0, 
	0x00, 0x00, 0x07, 0xc0, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 
	0x03, 0xf0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0xff, 
	0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapAlarmOff [] PROGMEM = {
	// 'ic_alarm_off_black_24dp_2x, 56x56px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x60, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x00, 0xf0, 0x70, 0x00, 0x07, 0xc0, 0x00, 0x00, 0xf8, 0x20, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x7c, 0x00, 0xff, 0x03, 0xf8, 0x00, 0x00, 0x3e, 0x07, 0xff, 0xe1, 
	0xfc, 0x00, 0x00, 0x7f, 0x07, 0xff, 0xf8, 0x7e, 0x00, 0x00, 0xff, 0x83, 0xff, 0xfe, 0x3f, 0x00, 
	0x00, 0x7f, 0xc1, 0x00, 0xff, 0x1e, 0x00, 0x00, 0x33, 0xe0, 0x00, 0x3f, 0x8c, 0x00, 0x00, 0x03, 
	0xf0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x07, 0xfc, 0x00, 
	0x03, 0xe0, 0x00, 0x00, 0x0f, 0xbe, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x1f, 0x00, 0x00, 0xf0, 
	0x00, 0x00, 0x1f, 0x0f, 0x80, 0x00, 0xf8, 0x00, 0x00, 0x1e, 0x07, 0xc0, 0x00, 0x78, 0x00, 0x00, 
	0x1e, 0x03, 0xe0, 0x00, 0x78, 0x00, 0x00, 0x3c, 0x01, 0xf0, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
	0xf8, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x7c, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x3e, 0x00, 
	0x3c, 0x00, 0x00, 0x3c, 0x00, 0x1f, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0x80, 0x3c, 0x00, 
	0x00, 0x3c, 0x00, 0x07, 0xc0, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x03, 0xe0, 0x3c, 0x00, 0x00, 0x1e, 
	0x00, 0x01, 0xf0, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x00, 0xf8, 0x38, 0x00, 0x00, 0x1f, 0x00, 0x00, 
	0x7c, 0x18, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x1f, 0x00, 
	0x00, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xc0, 0x00, 0x00, 
	0x03, 0xf0, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0xff, 
	0x00, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x7c, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 
	0x3c, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x1c, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x08, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/******************************************************************************/
ImageButton::ImageButton(int left, int top, ButtonImage image, int id, Colours colour): 
    Button(left, top) {
    this->id = id;
    this->width = GRID_WIDTH;
    this->height = GRID_HEIGHT;
	this->colour = colour;

	setImage(image);
}

ImageButton::ImageButton(int left, int top, ButtonImage image, int id):
	ImageButton(left, top, image, id, Colours::DarkGreen) {}

ImageButton::~ImageButton() {
}

void ImageButton::showDown() {
    //Output.drawRectangle(left, top, width, height, Colours::Green);
}

void ImageButton::showUp() {
    //Output.drawRectangle(left, top, width, height, Colours::White);
}

void ImageButton::setImage(ButtonImage image) {
    switch (image) {
        case ButtonImage::Remote: pImage = bitmapPower; break;
        case ButtonImage::Settings: pImage = bitmapSettings; break;
        case ButtonImage::SetAlarm: pImage = bitmapAlarm; break;
        case ButtonImage::Checked: pImage = bitmapChecked; break;
        case ButtonImage::Unchecked: pImage = bitmapUnchecked; break;
        case ButtonImage::Snooze: pImage = bitmapSnooze; break;
        case ButtonImage::AlarmOff: pImage = bitmapAlarmOff; break;
        case ButtonImage::Back: pImage = bitmapBack; break;

        case ButtonImage::Save: pImage = bitmapSave; this->width = GRID_WIDTH; this->height = GRID_HEIGHT; break;
    }
	
    Output.drawImage(left, top, width, height, colour, pImage);	
}

/******************************************************************************/
BitmapButton::BitmapButton (int left, int top, int width, int height, const char* pfilename, int id)
    : Button(left, top) {
    this->id = id;
    this->width = width;
    this->height = height;

	Output.drawBitmap(left, top, pfilename);
}

BitmapButton::~BitmapButton() {}

void BitmapButton::showDown() {
    Output.drawRectangle(left, top, width, height, Colours::Green);	
}

void BitmapButton::showUp() {
    Output.drawRectangle(left, top, width, height, Colours::White);	
}

/******************************************************************************/
TextButton::TextButton(int left, int top, int width, int height, const char* ptext, int id)
	: Button(left, top, width, height) {
	this->id = id;

	Output.showCentredText(left, top, width, height, ptext, Colours::Blue);
}

TextButton::~TextButton() {}