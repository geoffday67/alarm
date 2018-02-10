#include <stdio.h>
#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"

extern Screen* pIdleScreen;

class RemoteScreen: public Screen, EventReceiver {
public:
    RemoteScreen (): Screen () {    }
    virtual ~RemoteScreen() {}

    virtual void activate() {
        EventManager.addListener(EVENT_BUTTON, this);
        Output.clear();
        pBackButton = new ImageButton(8, 168, ButtonImage::Back, 0);
        pOnButton = new BitmapButton(149, 40, 128, 160, "/kettle.bmp", 1);
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
            case 0:
                this->deactivate();
                pIdleScreen->activate();
                break;
        }
    }
};

Screen* createRemoteScreen () {
        return new RemoteScreen();
    }
