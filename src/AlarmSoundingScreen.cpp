#include "AlarmSoundingScreen.h"

classAlarmSoundingScreen::classAlarmSoundingScreen () {}

classAlarmSoundingScreen::~classAlarmSoundingScreen() {}

extern Screen* pIdleScreen;

classAlarmSoundingScreen AlarmSoundingScreen;

#define ALARM_OFF_BUTTON        1
#define SNOOZE_BUTTON           2

void classAlarmSoundingScreen::activate() {
    Output.clear();
	Output.drawBitmap(0, 0, "/alarm.bmp");
    Output.startAlarm();
    
    EventManager.addListener(EVENT_BUTTON, this);

    pStopButton = new ImageButton(LEFT_BUTTON_X, LEFT_BUTTON_Y, ButtonImage::AlarmOff, ALARM_OFF_BUTTON);
    pSnoozeButton = new ImageButton(RIGHT_BUTTON_X, RIGHT_BUTTON_Y, ButtonImage::Snooze, SNOOZE_BUTTON);
}

void classAlarmSoundingScreen::deactivate() {
    Output.stopAlarm();

    EventManager.removeListener(this);

    delete pStopButton;
    delete pSnoozeButton;
}

void classAlarmSoundingScreen::onEvent(Event* pevent) {
    if (pevent->type != EVENT_BUTTON)
        return;

    ButtonEvent *pbutton = (ButtonEvent*) pevent;

    switch (pbutton->id) {
        case ALARM_OFF_BUTTON:
            this->deactivate();
            pIdleScreen->activate();            
            break;
    }
}