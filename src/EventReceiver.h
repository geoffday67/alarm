#ifndef __EVENT_RECEIVER__
#define __EVENT_RECEIVER__

class EventReceiver {
public:
    virtual void onEvent(int event, void *pdata) = 0;
};

#endif