#ifndef __BUTTON__
#define __BUTTON__

#include "EventReceiver.h"
#include "Event.h"
#include "Output.h"

class Button: public EventReceiver {
  protected:
    Button(int left, int top);
    int left, top, width, height;

  public:
    Button(int left, int top, int width, int height);
    virtual ~Button();
    virtual bool contains(int x, int y);
    virtual void showDown();
    virtual void showUp();
    virtual void onEvent(Event* pevent);

    int id;   
};

enum ButtonImage {
  Save, Remote, Back, Settings, Checked, Unchecked, Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Backspace, A, P
};

class ImageButton: public Button {
  private:
    const unsigned char* pImage;
    Colours colour;

  public:
    ImageButton (int left, int top, ButtonImage image, int id);
    ImageButton (int left, int top, ButtonImage image, int id, Colours colour);
    virtual ~ImageButton();
    virtual void showDown();
    virtual void showUp();
    void setImage(ButtonImage image);
};

class BitmapButton: public Button {
public:
    BitmapButton (int left, int top, int width, int height, const char* pfilename, int id);
    virtual ~BitmapButton();
    virtual void showDown();
    virtual void showUp();
};

class TextButton: public Button {
public:
  TextButton (int left, int top, int width, int height, const char* ptext, int id);
  virtual ~TextButton();
};

#endif