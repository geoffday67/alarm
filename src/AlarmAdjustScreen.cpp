#include "AlarmAdjustScreen.h"
#include "Button.h"
#include "Output.h"
#include "AlarmManager.h"

extern Screen* pAlarmSetScreen;

classAlarmAdjustScreen AlarmAdjustScreen;

#define BACK_BUTTON                 -1
#define BACKSPACE_BUTTON            -2
#define AM_BUTTON                   -3
#define PM_BUTTON                   -4
#define SAVE_BUTTON                 -5

classAlarmAdjustScreen::classAlarmAdjustScreen () {}

classAlarmAdjustScreen::~classAlarmAdjustScreen() {}

void classAlarmAdjustScreen::showEntry() {
    int x = LEFT_MARGIN;
    int y = TOP_MARGIN;

    char s[2];
    s[1] = 0;

    for (int n = 0; n < 4; x += GRID_WIDTH, n++) {
        if (entry[n] == -1) {
            Output.showText(x, y, "_", Colours::Green, Colours::White);
            continue;
        }
        s[0] = entry[n] + 48;
        Output.showText(x, y, s, Colours::Green, Colours::White);
    }

    Output.showText(x, y, am ? "AM" : "PM", Colours::Green, Colours::White);    
}

void classAlarmAdjustScreen::activate() {
    Output.clear();
    EventManager.addListener(EVENT_BUTTON, this);
    pBackButton = new ImageButton(LEFT_BUTTON_X, LEFT_BUTTON_Y, ButtonImage::Back, BACK_BUTTON);
    pSaveButton = new ImageButton(RIGHT_BUTTON_X, RIGHT_BUTTON_Y, ButtonImage::Save, SAVE_BUTTON);

    position = 0;
    entry[0] = -1;
    entry[1] = -1;
    entry[2] = -1;
    entry[3] = -1;
    am = true;
    showEntry();

    // Show buttons for data entry
    digitButtons[1] = new ImageButton (20, 120, ButtonImage::One, 1, Colours::Blue);
    digitButtons[2] = new ImageButton (76, 120, ButtonImage::Two, 2, Colours::Blue);
    digitButtons[3] = new ImageButton (132, 120, ButtonImage::Three, 3, Colours::Blue);
    digitButtons[4] = new ImageButton (188, 120, ButtonImage::Four, 4, Colours::Blue);
    digitButtons[5] = new ImageButton (244, 120, ButtonImage::Five, 5, Colours::Blue);

    digitButtons[6] = new ImageButton (20, 64, ButtonImage::Six, 6, Colours::Blue);
    digitButtons[7] = new ImageButton (76, 64, ButtonImage::Seven, 7, Colours::Blue);
    digitButtons[8] = new ImageButton (132, 64, ButtonImage::Eight, 8, Colours::Blue);
    digitButtons[9] = new ImageButton (188, 64, ButtonImage::Nine, 9, Colours::Blue);
    digitButtons[0] = new ImageButton (244, 64, ButtonImage::Zero, 0, Colours::Blue);    

    digitButtons[10] = new ImageButton (76, 176, ButtonImage::Backspace, BACKSPACE_BUTTON, Colours::Blue);    
    digitButtons[11] = new ImageButton (132, 176, ButtonImage::A, AM_BUTTON, Colours::Blue);    
    digitButtons[12] = new ImageButton (188, 176, ButtonImage::P, PM_BUTTON, Colours::Blue);    
}

void classAlarmAdjustScreen::deactivate() {
    for (int n = 0; n < 10; n++) {
        delete digitButtons[n];
    }
    delete pBackButton;
    delete pSaveButton;
    EventManager.removeListener(this);
}

void classAlarmAdjustScreen::onEvent(Event* pevent) {
    if (pevent->type != EVENT_BUTTON)
        return;

    ButtonEvent *pbutton = (ButtonEvent*) pevent;

    // Check for 'back' pressed
    if (pbutton->id == BACK_BUTTON) {
        this->deactivate();
        pAlarmSetScreen->activate();
        return;
    }

    if (pbutton->id == SAVE_BUTTON) {
        Alarm alarm;
        alarm.hour = (entry[0] * 10) + entry[1];
        if (!am) alarm.hour += 12;
        alarm.minute = (entry[2] * 10) + entry[3];
        AlarmManager.setAlarm (index, &alarm);

        this->deactivate();
        pAlarmSetScreen->activate();
        return;
    }    

    if (pbutton->id == BACKSPACE_BUTTON) {
        if (position > 0) {
            entry[--position] = -1;
            showEntry();
        }
        return;
    }

    if (pbutton->id == AM_BUTTON) {
        if (!am) {
            am = true;
            showEntry();
        }
        return;
    }

    if (pbutton->id == PM_BUTTON) {
        if (am) {
            am = false;
            showEntry();
        }
        return;
    }

    if (position > 3) {
        return;
    }

    entry[position++] = pbutton->id;
    showEntry();
}

void classAlarmAdjustScreen::setAlarm (Alarm* palarm) {
    this->pAlarm = palarm;
}

void classAlarmAdjustScreen::setAlarmIndex (int index) {
     this->index = index;
}
