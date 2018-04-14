#ifndef __PERSISTENT__
#define __PERSISTENT__

#include "Calibration.h"
#include "AlarmManager.h"

class classPersistent {
private:
    int next_address;

public:
    classPersistent();
    int allocate (int size);
    void store (int address, void* pdata, int size);
    void fetch (int address, void* pdata, int size);

    void storeCalibration (CalibrationData *pdata); 
    void fetchCalibration (CalibrationData *pdata);

    void storeAlarms(Alarm* palarm, int count);
    void fetchAlarms(Alarm* palarm, int count);
};

extern classPersistent Persistent;

#endif
