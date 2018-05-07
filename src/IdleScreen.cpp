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

#define SNOOZE_BUTTON           2

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
        if (pAlarm) {
            alarmTime = AlarmManager.getAlarmTime(pAlarm);
            if (pAlarm->isSnoozing()) {
                pSnoozeButton = new ImageButton(CENTRE_BUTTON_X, CENTRE_BUTTON_Y, ButtonImage::Snooze, SNOOZE_BUTTON, Colours::Red);
            }
        }

        showTime();
        showAlarmTime();
    }

    virtual void deactivate() {
        EventManager.removeListener(this);
        delete pRemoteButton;
        delete pAlarmButton;
        delete pSnoozeButton;
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
    Button* pSnoozeButton;
    time_t previousTime;
    time_t alarmTime;
    Alarm* pAlarm;

void printTime(time_t time) {
tmElements_t elements;
breakTime(time, elements);
char text [32];
sprintf (text, "%02d-%02d-%04d %02d:%02d:%02d", elements.Day, elements.Month, elements.Year + 1970, elements.Hour, elements.Minute, elements.Second);
Serial.print(text);
}

    void handleTimeEvent (TimeEvent *pevent) {
        showTime();

        time_t now = AlarmManager.getCurrentTime();

        // Check if an alarm has gone off
        if (pAlarm && previousTime < alarmTime && now >= alarmTime) {
            this->deactivate();
            AlarmSoundingScreen.setAlarm(pAlarm);
            AlarmSoundingScreen.activate();
        }

        previousTime = now;
    }

    void showTime() {
        tmElements_t elements;
        breakTime(AlarmManager.getCurrentTime(), elements);

        char text [32];
        sprintf (text, "%02d:%02d:%02d", elements.Hour, elements.Minute, elements.Second);
        Output.showText(SCREEN_CENTRE, TOP_MARGIN + 40, text, Colours::Blue);       
    }

    void showAlarmTime() {
        char text [32];

        if (pAlarm) {
            tmElements_t elements;
            breakTime(alarmTime, elements);
            sprintf (text, "%02d:%02d", elements.Hour, elements.Minute);
        } else {
            strcpy (text, "     ");
        }

        Output.showText(SCREEN_CENTRE, TOP_MARGIN + 110, text, Colours::LightGrey);               
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

            case SNOOZE_BUTTON:
                Output.fillRectangle(CENTRE_BUTTON_X, CENTRE_BUTTON_Y, GRID_WIDTH, GRID_HEIGHT, Colours::White);
                delete pSnoozeButton;
                pSnoozeButton = NULL;

                AlarmManager.resetSnooze(pAlarm);
                alarmTime = AlarmManager.getAlarmTime(pAlarm);
                showAlarmTime();

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
