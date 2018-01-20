#ifndef __BUTTON__
#define __BUTTON__

#include "EventReceiver.h"
#include "Event.h"
#include "Output.h"

class Button: public EventReceiver {
  protected:
    Button(Output *poutput, int left, int top);
    int _id;
    Output *pOutput;
    int left, top, width, height;

  public:
    Button(Output *poutput, int left, int top, int width, int height);
    virtual ~Button();
    Button* id(int);
    virtual bool contains(int x, int y);
    virtual void showDown();
    virtual void showUp();
    virtual void onEvent(Event* pevent);
    virtual void identify();
};

enum ButtonImage {
  Save, Remote, Back, Settings
};

class ImageButton: public Button {
  private:
    const unsigned char* pImage;

  public:
    ImageButton (Output *poutput, int left, int top, ButtonImage image, int id);
    virtual ~ImageButton();
    virtual void showDown();
    virtual void showUp();
};

class BitmapButton: public Button {
public:
    BitmapButton (Output *poutput, int left, int top, int width, int height, const char* pfilename, int id);
    virtual ~BitmapButton();
    virtual void showDown();
    virtual void showUp();
};

class TextButton: public Button {
public:
  TextButton (Output *poutput, int left, int top, int width, int height, const char* ptext, int id);
  virtual ~TextButton();
};

#endif