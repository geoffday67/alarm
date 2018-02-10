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

#define SAVE_BUTTON                 -1
#define ACTIVE_BUTTON_FIRST         10
#define ALARM_BUTTON_FIRST          20

class AlarmSetScreen: public Screen, EventReceiver {
private:
    Alarm currentAlarms[ALARM_COUNT];
    TextButton *pAlarmButtons[ALARM_COUNT];
    RadioButtons* pActiveButtons;
    Button *pSaveButton;

    void showAlarms() {
        // Display the configured alarms as text buttons
        int y = TOP_MARGIN;
        char text [32];
        Alarm *palarm;
        for (int n = 0; n < ALARM_COUNT; n++) {
            palarm = currentAlarms + n;

            if (palarm->configured) {
                sprintf (text, "%2d:%02d %s", palarm->hour, palarm->minute, palarm->isAM() ? "AM" : "PM");
            } else {
                strcpy (text, "");
            }

            pAlarmButtons[n] = new TextButton (80, y, 160, 48, text, n + ALARM_BUTTON_FIRST);
            pActiveButtons->add(252, y);

            if (palarm->enabled) {
                pActiveButtons->setChecked(n);
            }

            y += 58;
        }
    }

public:
    AlarmSetScreen (): Screen () {}
    virtual ~AlarmSetScreen() {}

    virtual void activate() {
        pActiveButtons = new RadioButtons(ACTIVE_BUTTON_FIRST);
        pActiveButtons->canToggle = true;

        Output.clear();

        AlarmManager.getAlarms(currentAlarms);
        showAlarms();

        EventManager.addListener(EVENT_BUTTON, this);
        pSaveButton = new ImageButton(LEFT_BUTTON_X, LEFT_BUTTON_Y, ButtonImage::Save, SAVE_BUTTON);
    }

    virtual void deactivate() {
        delete pActiveButtons;
        for (int n = 0; n < ALARM_COUNT; n++) {
            delete pAlarmButtons[n];
        }
        delete pSaveButton;
        EventManager.removeListener(this);
    }

    virtual void onEvent(Event *pevent) {
        if (pevent->type != EVENT_BUTTON)
            return;

        ButtonEvent *pbutton = (ButtonEvent*) pevent;

        if (pbutton->id == SAVE_BUTTON) {
            int checked = pActiveButtons->getChecked();
            for (int n = 0; n < ALARM_COUNT; n++) {
                currentAlarms[n].enabled = (n == checked);
                AlarmManager.setAlarm(n, currentAlarms + n);
            }                

            this->deactivate();
            pIdleScreen->activate();
            return;
        }

        // Check for one of the alarm times pressed
        if (pbutton->id >= ALARM_BUTTON_FIRST && pbutton->id < ALARM_BUTTON_FIRST + ALARM_COUNT) {
            this->deactivate();
            //AlarmAdjustScreen.setAlarm(currentAlarms + pbutton->id - ALARM_BUTTON_FIRST);
            AlarmAdjustScreen.setAlarmIndex (pbutton->id - ALARM_BUTTON_FIRST);
            AlarmAdjustScreen.activate();
            return;
        }
    }
};

Screen* createAlarmSetScreen () {
        return new AlarmSetScreen();
    }
