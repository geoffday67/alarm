#include <stdio.h>
#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"
#include "WiFiManager.h"

extern Screen* pIdleScreen;

#define     BACK_BUTTON         1
#define     KETTLE_BUTTON       2

class RemoteScreen: public Screen, EventReceiver {
public:
    RemoteScreen (): Screen () {    }
    virtual ~RemoteScreen() {}

    virtual void activate() {
        EventManager.addListener(EVENT_BUTTON, this);
        Output.clear();
        pBackButton = new ImageButton(LEFT_BUTTON_X, LEFT_BUTTON_Y, ButtonImage::Back, BACK_BUTTON);
        pOnButton = new BitmapButton(140, 40, 128, 160, "/kettle.bmp", KETTLE_BUTTON);
    }

    virtual void deactivate() {
        EventManager.removeListener(this);
        delete pBackButton;
        delete pOnButton;
    }

    virtual void onEvent(Event* pevent) {
        switch (pevent->type) {
            case EVENT_BUTTON:
                handleButtonEvent((ButtonEvent*) pevent);
                break;
        }
    }

    virtual void identify() {
        Serial.print("RemoteScreen");
    }

private:
    Button *pBackButton;
    Button *pOnButton;

    void handleButtonEvent (ButtonEvent *pevent) {
        switch(pevent->id) {
            case BACK_BUTTON:
                this->deactivate();
                pIdleScreen->activate();
                break;

            case KETTLE_BUTTON:
                WiFiManager.sendCommand(3, "on");
                this->deactivate();
                pIdleScreen->activate();
                break;
        }
    }
};

Screen* createRemoteScreen () {
        return new RemoteScreen();
    }
