#include "EventManager.h"
#include "RadioButtons.h"

using namespace std;

RadioButtons::RadioButtons(int firstId) {
    canToggle = false;
    this->firstId = firstId;
    nextId = firstId;
    EventManager.addListener(EVENT_BUTTON, this);
}

RadioButtons::~RadioButtons() {
    for (int n = 0; n < vButtons.size(); n++) {
        delete vButtons[n];
    }
    EventManager.removeListener(this);
}

void RadioButtons::add (int x, int y) {
    vButtons.push_back(new ImageButton (x, y, ButtonImage::Unchecked, nextId++));
    vChecked.push_back(false);
}

void RadioButtons::onEvent(Event *pevent) {
    if (pevent->type != EVENT_BUTTON)
        return;

    ButtonEvent* pbutton_event = (ButtonEvent*) pevent;

    if (pbutton_event->id < firstId || pbutton_event->id >= nextId)
        return;

    for (int n = 0; n < vButtons.size(); n++) {
        ImageButton* pimage_button = vButtons[n];
        if (pimage_button->id == pbutton_event->id) {
            if (canToggle && isChecked (n)) {
                pimage_button->setImage(ButtonImage::Unchecked);            
                vChecked[n] = false;
            } else {
                pimage_button->setImage(ButtonImage::Checked);
                vChecked[n] = true;
            }
        } else {
            pimage_button->setImage(ButtonImage::Unchecked);            
            vChecked[n] = false;
        }
    }
}

void RadioButtons::setChecked (int index) {
    vButtons[index]->setImage(ButtonImage::Checked);
    vChecked[index] = true;
}

bool RadioButtons::isChecked (int index) {
    return vChecked[index];
}

int RadioButtons::getChecked() {
    for (int n = 0; n < vButtons.size(); n++) {
        if (vChecked[n])
            return n;
    }

    return RADIO_NONE_CHECKED;
}
