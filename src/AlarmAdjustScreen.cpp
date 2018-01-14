#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "AlarmManager.h"

extern classEventManager EventManager;

class AlarmAdjustScreen: public Screen, EventReceiver {
    public:
        AlarmAdjustScreen (Alarm *palarm, Output *pout): Screen (pout) {}
        virtual ~AlarmAdjustScreen() {}

    virtual void activate() {
    }

    virtual void deactivate() {
    }

    virtual void onEvent(int type, void *pdata) {
    }
};

Screen* createAlarmAdjustScreen (Alarm *palarm, Output *pout) {
        return new AlarmAdjustScreen(palarm, pout);
    }
