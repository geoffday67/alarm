#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "AlarmManager.h"
#include "Button.h"

extern classEventManager EventManager;

class AlarmSetScreen: public Screen, EventReceiver {
private:
    Alarm *pAlarms;

    void showAlarms() {
        int y = 20;
        char text [32];
        Alarm *palarm;
        pOutput->clear();
        for (int n = 0; n < AlarmManager.getAlarmCount(); n++) {
            palarm = pAlarms + n;
            if (!palarm->configured)
                continue;
            sprintf (text, "%2d:%02d %s", palarm->hour, palarm->minute, palarm->isAM() ? "AM" : "PM");
            pOutput->showText(20, y, text, Colours::Green);
            Button *pbutton = (new Button(pOutput, 20, y, 200, 36))
                ->id(n);
            y += 40;
        }
    }

public:
    AlarmSetScreen (Output *pout): Screen (pout) {}
    virtual ~AlarmSetScreen() {}

    virtual void activate() {
        pAlarms = new Alarm[AlarmManager.getAlarmCount()];
        AlarmManager.getAlarms(pAlarms);
        showAlarms();
        EventManager.addListener(EVENT_BUTTON, this);
    }

    virtual void deactivate() {
        delete [] pAlarms;
    }

    virtual void onEvent(int type, void *pdata) {
        if (type != EVENT_BUTTON)
            return;

        ButtonEvent *pbe = (ButtonEvent*) pdata;

        Serial.println(pbe->id);
    }
};

Screen* createAlarmSetScreen (Output *pout) {
        return new AlarmSetScreen(pout);
    }
