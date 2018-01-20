#ifndef __ALARM_MANAGER__
#define __ALARM_MANAGER__

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
};

class classAlarmManager
{
  private:
    Alarm alarms[4];
    void restore();
    time_t currentTime;

  public:
    classAlarmManager();
    int getAlarmCount();
    void getAlarms(Alarm* palarm);
    const Alarm *getNextAlarm(time_t now);
    time_t getCurrentTime();
    void setCurrentTime(time_t time);
};

extern classAlarmManager AlarmManager;

#endif
