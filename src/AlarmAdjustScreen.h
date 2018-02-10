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
    Alarm* pAlarm;
    Button *pBackButton;
    Button *pSaveButton;
    Button* digitButtons[13];
    int position;
    int entry[4];
    bool am;
    int index;
    void showEntry();
    
public:
    classAlarmAdjustScreen ();
    virtual ~classAlarmAdjustScreen();

    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);

    void setAlarm (Alarm* palarm);
    void setAlarmIndex (int index);
};

extern classAlarmAdjustScreen AlarmAdjustScreen;

#endif