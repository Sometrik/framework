#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <UIElement.h>

#include <ValueEvent.h>
#include <Command.h>
#include <CommandEvent.h>
#include <TouchEvent.h>

class Button : public UIElement {
 public:
 Button(const std::string & _label, int _id = 0, unsigned int _flags = 0)
   : UIElement(_id, _flags), label(_label) { }

  bool isA(const std::string & className) override {
    if (className == "Button") return true;
    return UIElement::isA(className);
  }

  void onTouchEvent(TouchEvent & ev) override {
    if (ev.getType() == TouchEvent::ACTION_DOWN &&
	ev.getX() >= x && ev.getX() < x + width &&
	ev.getY() >= y && ev.getY() < y + height) {
      setTouched(true);
      CommandEvent ev2(ev.getTimestamp(), getId());
      ev2.dispatch(*this);
      ev.setHandled();
      ev.requestRedraw();
    } else if (ev.getType() == TouchEvent::ACTION_UP && isTouched()) {
      setTouched(false);
      ev.setHandled();
      ev.requestRedraw();
    }
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(ev.getTimestamp(), getId());
    ev2.dispatch(*this);
  }
  
  void renderContent() override {
    if (context) {
      context->renderButton(width, height, isTouched());
    }
  }

  void setLabel(const std::string & l) {
    if (l != label) {
      label = l;
      if (context) context->clear();
    }
  }

  bool call() override {
    sendCommand(Command(Command::SET_INT_VALUE, getInternalId()));
    return true;
  }
  bool call(bool t) override { return call(); }
  bool call(int i) override { return call(); }
  bool call(const std::string & s) override { return call(); }

protected:
  void create() override {
    Command c(Command::CREATE_BUTTON, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
  }

 private:
  std::string label;
};

#endif
