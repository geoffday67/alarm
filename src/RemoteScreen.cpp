#include <stdio.h>
#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"
#include "Output.h"
#include "Button.h"

extern classEventManager EventManager;
extern Screen* pIdleScreen;

class RemoteScreen: public Screen, EventReceiver {
public:
    RemoteScreen (Output *pout): Screen (pout) {
    }
    virtual ~RemoteScreen() {}

    virtual void activate() {
        EventManager.addListener(EVENT_BUTTON, this);
        pOutput->clear();
        pBackButton = (new ImageButton(pOutput, 8, 168, ButtonImage::Back, 99));
    }

    virtual void deactivate() {
        EventManager.removeListener(this);
        delete pBackButton;
    }

    virtual void onEvent(int type, void *pdata) {
        switch (type) {
            case EVENT_BUTTON:
                handleButtonEvent((ButtonEvent*) pdata);
                break;
        }
    }

private:
    Button *pBackButton;

    void handleButtonEvent (ButtonEvent *pevent) {
        this->deactivate();
        pIdleScreen->activate();
    }
};

Screen* createRemoteScreen (Output *pout) {
        return new RemoteScreen(pout);
    }
