#include "AlarmManager.h"
#include <string.h>

classAlarmManager AlarmManager;

bool Alarm::isAM() {
    return hour < 12;
}

classAlarmManager::classAlarmManager() {
    restore();
}

void classAlarmManager::restore() {
    alarms[0].hour = 7;
    alarms[0].minute = 0;
    alarms[0].enabled = true;
    alarms[0].configured = true;

    alarms[1].hour = 12;
    alarms[1].minute = 20;
    alarms[1].enabled = true;
    alarms[1].configured = true;

    alarms[2].configured = false;
    alarms[3].configured = false;
}

int classAlarmManager::getAlarmCount() {
    return 4;
}

void classAlarmManager::getAlarms(Alarm* palarm) {
    for (int n = 0; n < getAlarmCount(); n++) {
        memcpy (palarm + n, alarms + n, sizeof (Alarm));
    }
}

const Alarm* classAlarmManager::getNextAlarm(time_t now) {
    int found = -1;
    int min = 24 * 60;

    tmElements_t elements;
    breakTime(now, elements);
    int now_mins = (elements.Hour * 60) + elements.Minute;

    // For each configured alarm time, calculate its difference from now. Choose the least.
    for (int n = 0; n < getAlarmCount(); n++)
    {
        if (!alarms[n].configured) {
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
