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

Alarm* classAlarmManager::getNextAlarm() {
    // Only one alarm can be enabled at once so return that one (or none)
    for (int n = 0; n < ALARM_COUNT; n++) {
        if (alarms[n].enabled && alarms[n].configured)
            return alarms + n;
    }

    return NULL;
}

time_t classAlarmManager::getAlarmTime(const Alarm* palarm) {
    if (palarm->snooze != 0) {
        return palarm->snooze;
    }

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

void classAlarmManager::enableAlarm(Alarm* palarm) {
    resetSnooze(palarm);
    palarm->enabled = true;
}

void classAlarmManager::disableAlarm(Alarm* palarm) {
    palarm->enabled = false;
}

void classAlarmManager::resetSnooze(Alarm* palarm) {
    // Set the snooze back to zero so it isn't used
    palarm->snooze = 0;
}

void classAlarmManager::snoozeAlarm(Alarm* palarm, int minutes) {
    // Set the new alarm time for the current time plus the snooze amount
    palarm->snooze = currentTime + minutes * SECS_PER_MIN;
}

void classAlarmManager::setCurrentTime(time_t time) {
    currentTime = time;
}

time_t classAlarmManager::getCurrentTime() {
    return currentTime;
}
