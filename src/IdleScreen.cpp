#include <stdio.h>
#include <time.h>
#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"

extern classEventManager EventManager;
extern Screen* pRemoteScreen;

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
    }

    virtual void deactivate() {
        EventManager.removeListener(this);
        delete pRemoteButton;
    }

    virtual void onEvent(int type, void *pdata) {
        switch (type) {
            case EVENT_TIME:
                handleTimeEvent(*((time_t*) pdata));
                break;

            case EVENT_BUTTON:
                handleButtonEvent((ButtonEvent*) pdata);
                break;
        }
    }

private:
    Button* pRemoteButton;
    time_t previousTime;

    void handleTimeEvent (time_t now) {
        showTime(now);

        // Check if an alarm has gone off
        /*if (previousTime < pEventManager->getNextAlarmTime() &&
            now > pEventManager->getNextAlarmTime()) {
                pOutput->soundAlarm();
        }*/

        previousTime = now;
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
        this->deactivate();
        pRemoteScreen->activate();
    }
};

Screen* createIdleScreen (Output *pout) {
        return new IdleScreen(pout);
    }

