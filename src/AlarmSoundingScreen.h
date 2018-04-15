#ifndef __ALARM_SOUNDING_SCREEN__
#define __ALARM_SOUNDING_SCREEN__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Button.h"
#include "AlarmManager.h"

class classAlarmSoundingScreen: public Screen, EventReceiver {
private:
    Alarm* pAlarm;
    Button *pStopButton;
    Button *pSnoozeButton;

public:
    classAlarmSoundingScreen ();
    virtual ~classAlarmSoundingScreen();

    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);

    void setAlarm (Alarm*);
};

extern classAlarmSoundingScreen AlarmSoundingScreen;

#endif