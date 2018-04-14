#ifndef __ALARM_SOUNDING_SCREEN__
#define __ALARM_SOUNDING_SCREEN__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Button.h"

class classAlarmSoundingScreen: public Screen, EventReceiver {
private:
    Button *pStopButton;
    Button *pSnoozeButton;

public:
    classAlarmSoundingScreen ();
    virtual ~classAlarmSoundingScreen();

    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);
};

extern classAlarmSoundingScreen AlarmSoundingScreen;

#endif