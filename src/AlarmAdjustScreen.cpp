#include "AlarmAdjustScreen.h"
#include "Button.h"
#include "Output.h"
#include "AlarmManager.h"

extern Screen* pAlarmSetScreen;

classAlarmAdjustScreen AlarmAdjustScreen;

#define DIGIT_BUTTON_WIDTH          60
#define DIGIT_BUTTON_HEIGHT         68

classAlarmAdjustScreen::classAlarmAdjustScreen () {}

classAlarmAdjustScreen::~classAlarmAdjustScreen() {}

void classAlarmAdjustScreen::showEntry() {
    char text[6];

    strcpy (text, "--:--");
    if (entry[0] != -1) text[0] = entry[0] + 48;
    if (entry[1] != -1) text[1] = entry[1] + 48;
    if (entry[2] != -1) text[3] = entry[2] + 48;
    if (entry[3] != -1) text[4] = entry[3] + 48;

    Output.showCentredText(0, TOP_MARGIN + 2 * (DIGIT_BUTTON_HEIGHT + 1), SCREEN_WIDTH - 1, 
        SCREEN_HEIGHT - (TOP_MARGIN + 2 * (DIGIT_BUTTON_HEIGHT + 1)), text, Colours::Red);
}

void classAlarmAdjustScreen::activate() {
    Output.clear();
    EventManager.addListener(EVENT_BUTTON, this);

    position = 0;
    entry[0] = -1;
    entry[1] = -1;
    entry[2] = -1;
    entry[3] = -1;
    showEntry();

    // Show buttons for data entry
    digitButtons[1] = new TextButton (LEFT_MARGIN + 0 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "1", 1);
    digitButtons[2] = new TextButton (LEFT_MARGIN + 1 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "2", 2);
    digitButtons[3] = new TextButton (LEFT_MARGIN + 2 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "3", 3);
    digitButtons[4] = new TextButton (LEFT_MARGIN + 3 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "4", 4);
    digitButtons[5] = new TextButton (LEFT_MARGIN + 4 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "5", 5);

    digitButtons[6] = new TextButton (LEFT_MARGIN + 0 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN + DIGIT_BUTTON_HEIGHT + 1, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "6", 6);
    digitButtons[7] = new TextButton (LEFT_MARGIN + 1 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN + DIGIT_BUTTON_HEIGHT + 1, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "7", 7);
    digitButtons[8] = new TextButton (LEFT_MARGIN + 2 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN + DIGIT_BUTTON_HEIGHT + 1, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "8", 8);
    digitButtons[9] = new TextButton (LEFT_MARGIN + 3 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN + DIGIT_BUTTON_HEIGHT + 1, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "9", 9);
    digitButtons[0] = new TextButton (LEFT_MARGIN + 4 * (DIGIT_BUTTON_WIDTH + 1), TOP_MARGIN + DIGIT_BUTTON_HEIGHT + 1, DIGIT_BUTTON_WIDTH, DIGIT_BUTTON_HEIGHT, "0", 0);
}

void classAlarmAdjustScreen::deactivate() {
    for (int n = 0; n < 10; n++) {
        delete digitButtons[n];
    }

    EventManager.removeListener(this);
}

void classAlarmAdjustScreen::onEvent(Event* pevent) {
    if (pevent->type != EVENT_BUTTON)
        return;

    ButtonEvent *pbutton = (ButtonEvent*) pevent;

    entry[position++] = pbutton->id;
    showEntry();

    if (position > 3) {
        adjustAlarm();
        this->deactivate();
        pAlarmSetScreen->activate();            
    }
}

void classAlarmAdjustScreen::setAlarm (Alarm* palarm) {
    this->pAlarm = palarm;
}

void classAlarmAdjustScreen::adjustAlarm() {
    int hour = (entry[0] * 10) + entry[1];
    int minute = (entry[2] * 10) + entry[3];

    if (hour <= 23 && minute <= 59) {
        AlarmManager.setAlarm (pAlarm, hour, minute);
    }
}
