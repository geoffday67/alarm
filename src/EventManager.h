#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__

#include <vector>
#include <time.h>
#include "EventReceiver.h"

#define NO_ALARM        0
#define EVENT_TIME      1
#define EVENT_ALARM_SET 2
#define EVENT_TOUCH     3
#define EVENT_BUTTON    4

class TouchEvent  {
    public:
        int x, y;
        bool down;
        TouchEvent(int x, int y, bool down): x(x), y(y), down(down) {}
};

class ButtonEvent {
    public:
        int id;
        ButtonEvent(int id): id(id) {}
};

class RegisteredReceiver {
    public:
        RegisteredReceiver(EventReceiver *per, int type): pEventReceiver(per), type(type) {}
        EventReceiver *pEventReceiver;
        int type;
};

class classEventManager {
public:
    classEventManager() {
    }

    void addListener(int type, EventReceiver *per) {
        listReceivers.emplace_back(per, type);
        Serial.println("Event listener added");
    }

    void removeListener(EventReceiver *per) {
        if (listReceivers.empty())
            return;

        std::vector<RegisteredReceiver>::iterator i = listReceivers.begin();
        while (i != listReceivers.end()) {
            if ((*i).pEventReceiver == per) {
                i = listReceivers.erase(i);
                Serial.println("Event listener removed");
            } else {
                i++;
            }
        }
    }

    void fireEvent (int type, void *pdata) {
        if (listReceivers.empty())
            return;

        std::vector<EventReceiver*> targets;

        std::vector<RegisteredReceiver>::const_iterator i;
        for(i = listReceivers.cbegin(); i != listReceivers.cend(); i++) {
            if ((*i).type == type) {
                targets.push_back(i->pEventReceiver);
            }
        }

        std::vector<EventReceiver*>::const_iterator j;
        for(j = targets.cbegin(); j != targets.cend(); j++) {
            (*j)->onEvent(type, pdata);
        }
    }

private:
    std::vector<RegisteredReceiver> listReceivers;
};

#endif
