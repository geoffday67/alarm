#include "AlarmManager.h"
#include "Persistent.h"

#include <string.h>
#include <TimeLib.h>

classAlarmManager AlarmManager;

bool Alarm::isAM() {
    return hour < 12;
}

bool Alarm::isSnoozing() {
    return snooze != 0;
}

classAlarmManager::classAlarmManager() {
    restore();
}

void classAlarmManager::save() {
    Persistent.storeAlarms(alarms, ALARM_COUNT);
}

void classAlarmManager::restore() {
    Persistent.fetchAlarms(alarms, ALARM_COUNT);

    for (int n = 0; n < ALARM_COUNT; n++) {
        resetSnooze(alarms + n);
    }
}

Alarm* classAlarmManager::getAlarms() {
    return alarms;
}

void classAlarmManager::getAlarms(Alarm* palarm) {
    for (int n = 0; n < ALARM_COUNT; n++) {
        memcpy (palarm + n, alarms + n, sizeof (Alarm));
    }
}

void classAlarmManager::setAlarm(Alarm* palarm, int hour, int minute) {
    palarm->hour = hour;
    palarm->minute = minute;
    palarm->snooze = 0;
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
    palarm->snooze = currentTime + (minutes * SECS_PER_MIN);
    palarm->snooze = (palarm->snooze / 60) * 60;        // Set seconds to zero
}

void classAlarmManager::setCurrentTime(time_t time) {
    currentTime = time;
}

time_t classAlarmManager::getCurrentTime() {
    return currentTime;
}
