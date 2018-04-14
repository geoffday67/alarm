#include "Persistent.h"
#include <EEPROM.h>

#define CALIBRATION_ADDRESS     0
#define ALARM_ADDRESS           (CALIBRATION_ADDRESS + sizeof (CalibrationData))

classPersistent Persistent;

classPersistent::classPersistent () {
    EEPROM.begin (128);
    next_address = 0;
}

int classPersistent::allocate (int size) {
    int n = next_address;
    next_address += size;
    return n;
}

void classPersistent::store (int address, void* pdata, int size) {
    unsigned char *p = (unsigned char*) pdata;
    for (int n = 0; n < size; n++) {
        EEPROM.write(address++, p[n]);
    }
    EEPROM.commit();
}

void classPersistent::fetch (int address, void* pdata, int size) {
    unsigned char *p = (unsigned char*) pdata;
    for (int n = 0; n < size; n++) {
        p[n] = EEPROM.read(address++);
    }
}

void classPersistent::storeCalibration (CalibrationData *pdata) {
    store (CALIBRATION_ADDRESS, pdata, sizeof (CalibrationData));
}

void classPersistent::fetchCalibration (CalibrationData *pdata) {
    fetch (CALIBRATION_ADDRESS, pdata, sizeof (CalibrationData));
}

void classPersistent::storeAlarms(Alarm* palarm, int count) {
    store (ALARM_ADDRESS, palarm, count * sizeof (Alarm));
}

void classPersistent::fetchAlarms(Alarm* palarm, int count) {
    fetch (ALARM_ADDRESS, palarm, count * sizeof (Alarm));
}
