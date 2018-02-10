#ifndef __RADIO_BUTTONS__
#define __RADIO_BUTTONS__

#include "Button.h"
#include "Event.h"
#include <vector>

#define RADIO_NONE_CHECKED      -1

class RadioButtons: public EventReceiver {
private:
    std::vector<ImageButton*> vButtons;
    std::vector<bool> vChecked;
    int firstId, nextId;

public:
    RadioButtons(int firstId);
    virtual ~RadioButtons();
    void add (int x, int y);
    void setChecked (int index);
    int getChecked();
    bool isChecked (int index);
    void onEvent(Event *pevent);
    bool canToggle;
};

#endif