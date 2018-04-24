#ifndef __ALARM_MANAGER__
#define __ALARM_MANAGER__

#define ALARM_COUNT   2

#include <time.h>

class Alarm
{
  public:
    int hour;
    int minute;
    bool configured;
    bool enabled;
    bool isAM();
    bool isSnoozing();

    friend class classAlarmManager;

  private:
    time_t snooze;
};

class classAlarmManager
{
  private:
    Alarm alarms[ALARM_COUNT];
    time_t currentTime;

  public:
    classAlarmManager();
    void save();
    void restore();
    Alarm *getAlarm(int index);
    void getAlarms(Alarm* palarm);
    Alarm* getAlarms();
    Alarm *getNextAlarm();
    time_t getCurrentTime();
    void setCurrentTime(time_t time);
    time_t getAlarmTime(const Alarm* palarm);
    void setAlarm(Alarm* palarm, int hour, int minute);
    void snoozeAlarm(Alarm* palarm, int minutes);
    void resetSnooze(Alarm* palarm);
    void enableAlarm(Alarm* palarm);
    void disableAlarm(Alarm* palarm);
};

extern classAlarmManager AlarmManager;

#endif
