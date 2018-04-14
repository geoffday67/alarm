#include "AlarmManager.h"
#include "Persistent.h"

#include <string.h>
#include <TimeLib.h>

classAlarmManager AlarmManager;

bool Alarm::isAM() {
    return hour < 12;
}

classAlarmManager::classAlarmManager() {
    restore();
}

void classAlarmManager::save() {
    Persistent.storeAlarms(alarms, ALARM_COUNT);
}

void classAlarmManager::restore() {
    alarms[0].hour = 7;
    alarms[0].minute = 0;
    alarms[0].enabled = false;
    alarms[0].configured = true;

    alarms[1].hour = 8;
    alarms[1].minute = 0;
    alarms[1].enabled = true;
    alarms[1].configured = true;

    alarms[2].hour = 9;
    alarms[2].minute = 0;
    alarms[2].enabled = false;
    alarms[2].configured = true;
}

void classAlarmManager::setAlarm(int index, Alarm* palarm) {
    memcpy (alarms + index, palarm, sizeof (Alarm));
}

void classAlarmManager::getAlarms(Alarm* palarm) {
    for (int n = 0; n < ALARM_COUNT; n++) {
        memcpy (palarm + n, alarms + n, sizeof (Alarm));
    }
}

Alarm* classAlarmManager::getAlarm(int index) {
    return alarms + index;
}

const Alarm* classAlarmManager::getNextAlarm(time_t now) {
    int found = -1;
    int min = 24 * 60;

    tmElements_t elements;
    breakTime(now, elements);
    int now_mins = (elements.Hour * 60) + elements.Minute;

    // For each configured and enabled alarm time, calculate its difference from now. Choose the least.
    for (int n = 0; n < ALARM_COUNT; n++)
    {
        if (!alarms[n].configured || !alarms[n].enabled) {
            continue;
        }
        int alarm_mins = (alarms[n].hour * 60) + alarms[n].minute;
        int diff_mins = alarm_mins - now_mins;
        if (diff_mins < 0) {
            diff_mins += 24 * 60;
        }
        if (diff_mins < min) {
            min = diff_mins;
            found = n;
        }
    }
    if (found == -1) {
        return NULL;
    } else {
        return alarms + found;
    }
}

const Alarm* classAlarmManager::getNextAlarm() {
    return getNextAlarm(currentTime);
}

time_t classAlarmManager::getAlarmTime(const Alarm* palarm) {
    tmElements_t elements;
    breakTime(currentTime, elements);
    elements.Hour = palarm->hour;
    elements.Minute = palarm->minute;
    elements.Second = 0;
    time_t alarm_time = makeTime(elements);
    if (alarm_time < currentTime) {
        alarm_time += SECS_PER_DAY;
    }

    return alarm_time;
}

void classAlarmManager::setCurrentTime(time_t time) {
    currentTime = time;
}

time_t classAlarmManager::getCurrentTime() {
    return currentTime;
}
