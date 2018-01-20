#include <stdio.h>
#include <time.h>
#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"
#include "AlarmManager.h"

extern Screen* pRemoteScreen;
extern Screen* pAlarmSetScreen;

/*
Screen is 320 x 240, buttons are 64 x 64 by default.
*/

class IdleScreen: public Screen, EventReceiver {
public:
    IdleScreen (Output *pout): Screen (pout) {
        previousTime = 0;
    }
    virtual ~IdleScreen() {}

    virtual void activate() {
        EventManager.addListener(EVENT_TIME, this);
        EventManager.addListener(EVENT_BUTTON, this);
        pOutput->clear();
        pRemoteButton = (new ImageButton(pOutput, 8, 168, ButtonImage::Remote, 0));
        pSettingsButton = (new ImageButton(pOutput, 248, 168, ButtonImage::Settings, 1));
        showTime(AlarmManager.getCurrentTime());
    }

    virtual void deactivate() {
        EventManager.removeListener(this);
        delete pRemoteButton;
        delete pSettingsButton;
    }

    virtual void onEvent(Event* pevent) {
        switch (pevent->type) {
            case EVENT_TIME:
                handleTimeEvent((TimeEvent*) pevent);
                break;

            case EVENT_BUTTON:
                handleButtonEvent((ButtonEvent*) pevent);
                break;
        }
    }

    virtual void identify() {
        Serial.print("IdleScreen");
    }

private:
    Button* pRemoteButton;
    Button* pSettingsButton;
    time_t previousTime;

    void handleTimeEvent (TimeEvent *pevent) {
        showTime(pevent->time);

        // Check if an alarm has gone off
        /*if (previousTime < pEventManager->getNextAlarmTime() &&
            now > pEventManager->getNextAlarmTime()) {
                pOutput->soundAlarm();
        }*/

        previousTime = pevent->time;
    }

    void showTime(time_t time) {
        tmElements_t elements;
        breakTime(time, elements);
        char text [32];
        if (isAM(time)) {
            sprintf (text, "%d:%02d:%02d AM", elements.Hour, elements.Minute, elements.Second);
        } else {
            sprintf (text, "%d:%02d:%02d PM", elements.Hour - 12, elements.Minute, elements.Second);            
        }

        pOutput->showText(70, 40, text, Colours::Blue);       
    }

    void handleButtonEvent (ButtonEvent *pevent) {
        switch(pevent->id) {
            case 0:
                this->deactivate();
                pRemoteScreen->activate();
                break;

            case 1:
                this->deactivate();
                pAlarmSetScreen->activate();
                break;
        }
    }
};

Screen* createIdleScreen (Output *pout) {
        return new IdleScreen(pout);
    }
