#include "EventManager.h"
#include "Event.h"
#include <Arduino.h>

using namespace std;

classEventManager EventManager;

/*****************************************************************************/
/*****************************************************************************/
TimeEvent::TimeEvent(time_t time): Event(EVENT_TIME) {
    this->time = time;
}

/*****************************************************************************/
/*****************************************************************************/
TouchEvent::TouchEvent(int x, int y, bool down): Event(EVENT_TOUCH) {
    this->x = x;
    this->y = y;
    this->down = down;
}

/*****************************************************************************/
/*****************************************************************************/
ButtonEvent::ButtonEvent (int id): Event(EVENT_BUTTON) {
    this->id = id;
}

/*****************************************************************************/
/*****************************************************************************/
RegisteredReceiver::RegisteredReceiver(EventReceiver *per, int type) {
    this->pEventReceiver = per;
    this->type = type;
    this->fired = false;
}

/*****************************************************************************/
/*****************************************************************************/
void classEventManager::addListener(int type, EventReceiver *per) {
    listReceivers.emplace_back(per, type);
}

/*****************************************************************************/
void classEventManager::removeListener(EventReceiver *per) {
    if (listReceivers.empty()) {
        return;
    }

    vector<RegisteredReceiver>::iterator i = listReceivers.begin();
    while (i != listReceivers.end()) {
        if (i->pEventReceiver == per) {
            i = listReceivers.erase(i);
        } else {
            i++;
        }
    }
}

/*****************************************************************************/
void classEventManager::queueEvent (Event* pevent) {
    queueEvents.push(pevent);
}

/*****************************************************************************/
void classEventManager::processEvents() {
    // Get each event from the queue and fire all interested receivers.
    // So the queue is empty at the end - possibly limit number of events processed so it doesn't take too long??

    if (queueEvents.empty())
        return;

    vector<RegisteredReceiver> receivers(listReceivers);

    while (!queueEvents.empty()) {
        Event* pevent = queueEvents.front();
        queueEvents.pop();

        vector<RegisteredReceiver>::const_iterator i;
        for (i = receivers.cbegin(); i != receivers.cend(); i++) {
            if (i->type == pevent->type) {
                i->pEventReceiver->onEvent(pevent);
            }
        }

        delete pevent;
    }
}
