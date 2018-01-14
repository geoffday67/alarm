#ifndef __BUTTON__
#define __BUTTON__

#include "EventReceiver.h"
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
    virtual void onEvent(int type, void *pdata);
};

enum ButtonImage {
  Save, Remote, Back
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

#endif