#ifndef __ALARM_MANAGER__
#define __ALARM_MANAGER__

#define ALARM_COUNT   3

#include <time.h>
//#include "Persistent.h"

class Alarm
{
  public:
    int hour;
    int minute;
    bool configured;
    bool enabled;
    bool isAM();
    time_t snooze;
};

class classAlarmManager
{
  private:
    Alarm alarms[ALARM_COUNT];
    void save();
    void restore();
    time_t currentTime;

  public:
    classAlarmManager();
    void setAlarm(int index, Alarm* palarm);
    Alarm *getAlarm(int index);
    void getAlarms(Alarm* palarm);
    Alarm *getNextAlarm();
    time_t getCurrentTime();
    void setCurrentTime(time_t time);
    time_t getAlarmTime(const Alarm* palarm);
    void snoozeAlarm(Alarm* palarm, int minutes);
    void resetSnooze(Alarm* palarm);
    void enableAlarm(Alarm* palarm);
    void disableAlarm(Alarm* palarm);
};

extern classAlarmManager AlarmManager;

#endif
