#include <stdio.h>
#include <time.h>
#include <TimeLib.h>
#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"
#include "AlarmManager.h"
#include "AlarmSoundingScreen.h"

extern Screen* pRemoteScreen;
extern Screen* pAlarmSetScreen;

class IdleScreen: public Screen, EventReceiver {
public:
    IdleScreen (): Screen () {
        previousTime = 0;
    }
    virtual ~IdleScreen() {}

    virtual void activate() {
        EventManager.addListener(EVENT_TIME, this);
        EventManager.addListener(EVENT_BUTTON, this);
        Output.clear();
        pRemoteButton = (new ImageButton(LEFT_BUTTON_X, LEFT_BUTTON_Y, ButtonImage::Remote, 0));
        pAlarmButton = (new ImageButton(RIGHT_BUTTON_X, RIGHT_BUTTON_Y, ButtonImage::SetAlarm, 1));

        pAlarm = AlarmManager.getNextAlarm();
        alarmTime = AlarmManager.getAlarmTime(pAlarm);

        showTime();
        showAlarmTime();
    }

    virtual void deactivate() {
        EventManager.removeListener(this);
        delete pRemoteButton;
        delete pAlarmButton;
    }

    virtual void onEvent(Event* pevent) {
        switch (pevent->type) {
            case EVENT_TIME:
                handleTimeEvent((TimeEvent*) pevent);
                break;

            case EVENT_BUTTON:
                handleButtonEvent((ButtonEvent*) pevent);
                break;

            case EVENT_LIGHT:
                handleLightEvent((LightEvent*) pevent);
                break;
        }
    }

    virtual void identify() {
        Serial.print("IdleScreen");
    }

private:
    Button* pRemoteButton;
    Button* pAlarmButton;
    time_t previousTime;
    time_t alarmTime;
    const Alarm* pAlarm;

    void printTime(time_t time) {
    tmElements_t elements;
    breakTime(time, elements);

    char text [32];
    sprintf (text, "%02d-%02d-%04d %02d:%02d:%02d", elements.Day, elements.Month, elements.Year + 1970, elements.Hour, elements.Minute, elements.Second);

    Serial.println(text);
    }

    void handleTimeEvent (TimeEvent *pevent) {
        showTime();

        // Check if an alarm has gone off
        time_t now = AlarmManager.getCurrentTime();
        if (previousTime < alarmTime && now >= alarmTime) {
            this->deactivate();
            AlarmSoundingScreen.activate();
        }

        previousTime = now;
    }

    void showTime() {
        tmElements_t elements;
        breakTime(AlarmManager.getCurrentTime(), elements);

        char text [32];
        sprintf (text, "%02d:%02d:%02d", elements.Hour, elements.Minute, elements.Second);
        Output.showText(SCREEN_CENTRE, 40, text, Colours::Blue);       
    }

    void showAlarmTime() {
        char text [32];

        if (pAlarm) {
            sprintf (text, "%02d:%02d", pAlarm->hour, pAlarm->minute);
        } else {
            strcpy (text, "     ");
        }

        Output.showText(SCREEN_CENTRE, 110, text, Colours::LightGrey);               
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

    void handleLightEvent (LightEvent *pevent) {
        if (pevent->dark) {
            analogWrite (16, 16);
        } else {
            analogWrite (16, 1023);                    
        }
    }
};

Screen* createIdleScreen () {
        return new IdleScreen();
    }

