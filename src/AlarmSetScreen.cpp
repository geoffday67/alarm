#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "AlarmManager.h"
#include "Button.h"
#include "RadioButtons.h"
#include "AlarmAdjustScreen.h"

extern Screen* pIdleScreen;

using namespace std;

#define BACK_BUTTON                 -1
#define ACTIVE_BUTTON_FIRST         10
#define ALARM_BUTTON_FIRST          20

class AlarmSetScreen: public Screen, EventReceiver {
private:
    Alarm* pAlarms;
    TextButton *pAlarmButtons[ALARM_COUNT];
    RadioButtons* pActiveButtons;
    Button* pBackButton;

    void showAlarms() {
        int y = 36;
        char text [32];
        Alarm *palarm;
        for (int n = 0; n < ALARM_COUNT; n++) {
            palarm = pAlarms + n;

            if (palarm->configured) {
                sprintf (text, "%02d:%02d", palarm->hour, palarm->minute);
            } else {
                strcpy (text, "");
            }

            pAlarmButtons[n] = new TextButton (120, y, 160, 68, text, n + ALARM_BUTTON_FIRST);
            pActiveButtons->add(120 + 174, y + (68 - GRID_HEIGHT) / 2);

            if (palarm->enabled) {
                pActiveButtons->setChecked(n);
            }

            y += 78;
        }
    }

public:
    AlarmSetScreen (): Screen () {}
    virtual ~AlarmSetScreen() {}

    virtual void activate() {
        pAlarms = AlarmManager.getAlarms();

        pActiveButtons = new RadioButtons(ACTIVE_BUTTON_FIRST);
        pActiveButtons->canToggle = true;

        Output.clear();
        showAlarms();
        pBackButton = (new ImageButton(LEFT_BUTTON_X, LEFT_BUTTON_Y, ButtonImage::Back, BACK_BUTTON));

        EventManager.addListener(EVENT_BUTTON, this);
    }

    virtual void deactivate() {
        delete pActiveButtons;

        for (int n = 0; n < ALARM_COUNT; n++) {
            delete pAlarmButtons[n];
        }

        delete pBackButton;

        EventManager.removeListener(this);
    }

    virtual void onEvent(Event *pevent) {
        if (pevent->type != EVENT_BUTTON)
            return;

        ButtonEvent *pbutton = (ButtonEvent*) pevent;

        if (pbutton->id == BACK_BUTTON) {
            this->deactivate();
            pIdleScreen->activate();
            return;
        }

        // Check for one of the enable/disable button pressed
        if (pbutton->id >= ACTIVE_BUTTON_FIRST && pbutton->id < ACTIVE_BUTTON_FIRST + ALARM_COUNT) {
            int checked = pActiveButtons->getChecked();
            for (int n = 0; n < ALARM_COUNT; n++) {
                if (n == checked) {
                    AlarmManager.enableAlarm(pAlarms + n);
                } else {
                    AlarmManager.disableAlarm(pAlarms + n);
                }
            }                

            this->deactivate();
            pIdleScreen->activate();
            return;
        }

        // Check for one of the alarm times pressed
        if (pbutton->id >= ALARM_BUTTON_FIRST && pbutton->id < ALARM_BUTTON_FIRST + ALARM_COUNT) {
            this->deactivate();
            AlarmAdjustScreen.setAlarmIndex (pbutton->id - ALARM_BUTTON_FIRST);
            AlarmAdjustScreen.activate();
            return;
        }
    }
};

Screen* createAlarmSetScreen () {
    return new AlarmSetScreen();
}
