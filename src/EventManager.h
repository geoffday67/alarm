#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__

#include <vector>
#include <queue>
#include <time.h>
#include "Event.h"
#include "EventReceiver.h"

#define NO_ALARM        0

#define EVENT_TIME      1
#define EVENT_ALARM_SET 2
#define EVENT_TOUCH     3
#define EVENT_BUTTON    4

/*****************************************************************************/
class TimeEvent: public Event {
public:
    TimeEvent(time_t time);
    time_t time;
};

/*****************************************************************************/
class TouchEvent: public Event {
public:
    TouchEvent(int x, int y, bool down);
    int x, y;
    bool down;
};

/*****************************************************************************/
class ButtonEvent: public Event {
public:
    ButtonEvent(int id);
    int id;
};

/*****************************************************************************/
class RegisteredReceiver {
public:
    RegisteredReceiver(EventReceiver *per, int type);
    EventReceiver *pEventReceiver;
    int type;
    bool fired;
};

/*****************************************************************************/
class classEventManager {
public:
    void addListener(int type, EventReceiver *per);
    void removeListener(EventReceiver *per);
    void queueEvent (Event* pevent);
    void processEvents();

private:
    std::queue<Event*> queueEvents;
    std::vector<RegisteredReceiver> listReceivers;
};

extern classEventManager EventManager;

#endif
