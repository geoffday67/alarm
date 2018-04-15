#ifndef __ALARM_ADJUST_SCREEN__
#define __ALARM_ADJUST_SCREEN__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"
#include "AlarmManager.h"

class classAlarmAdjustScreen: public Screen, EventReceiver {
private:
    Button* digitButtons[13];
    int position;
    int entry[4];
    Alarm* pAlarm;
    void showEntry();
    void adjustAlarm();
    
public:
    classAlarmAdjustScreen ();
    virtual ~classAlarmAdjustScreen();

    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);

    void setAlarm(Alarm*);
};

extern classAlarmAdjustScreen AlarmAdjustScreen;

#endif