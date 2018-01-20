#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "AlarmManager.h"
#include "Button.h"
#include <vector>

extern Screen* pIdleScreen;

using namespace std;

class AlarmSetScreen: public Screen, EventReceiver {
private:
    Alarm *pAlarms;
    Button *pBackButton;
    vector<Button>* pButtons;

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
            pButtons->push_back(TextButton(pOutput, 20, y, 200, 40, text, n));
            y += 46;
        }
    }

public:
    AlarmSetScreen (Output *pout): Screen (pout) {}
    virtual ~AlarmSetScreen() {}

    virtual void activate() {
        pButtons = new vector<Button>();

        pAlarms = new Alarm[AlarmManager.getAlarmCount()];
        AlarmManager.getAlarms(pAlarms);
        showAlarms();
        EventManager.addListener(EVENT_BUTTON, this);
        pBackButton = new ImageButton(pOutput, 8, 168, ButtonImage::Back, 0);
    }

    virtual void deactivate() {
        delete pButtons;
        delete [] pAlarms;
        EventManager.removeListener(this);
        delete pBackButton;
    }

    virtual void onEvent(Event *pevent) {
        if (pevent->type != EVENT_BUTTON)
            return;

        ButtonEvent *pbutton = (ButtonEvent*) pevent;
        switch(pbutton->id) {
            case 0:
                this->deactivate();
                pIdleScreen->activate();
                break;
        }
    }

    virtual void identify() {
        Serial.print("AlarmSetScreen");
    }
};

Screen* createAlarmSetScreen (Output *pout) {
        return new AlarmSetScreen(pout);
    }
