#ifndef __EVENT_RECEIVER__
#define __EVENT_RECEIVER__

#include "Event.h"

class EventReceiver {
public:
    virtual void onEvent(Event* pevent) = 0;
};

#endif