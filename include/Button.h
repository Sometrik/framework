#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Element.h>

#include <ValueEvent.h>
#include <Command.h>
#include <CommandEvent.h>
#include <TouchEvent.h>

class Button : public Element {
 public:
 Button(const std::string & _label, int _id = 0, unsigned int _flags = 0)
   : Element(_id, _flags), label(_label) { }

  bool isA(const std::string & className) const override {
    if (className == "Button") return true;
    return Element::isA(className);
  }

  void onTouchEvent(TouchEvent & ev) override {
    if (ev.getType() == TouchEvent::ACTION_DOWN &&
	ev.getX() >= x && ev.getX() < x + width &&
	ev.getY() >= y && ev.getY() < y + height) {
      setTouched(true);
      CommandEvent ev2(getId());
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
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }
  
  void renderContent() override {
    if (context) {
      context->renderButton(width, height, isTouched());
    }
  }

  void text(const std::string & l) override {
    if (l != label) {
      label = l;
      if (context) context->clear();
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setTextValue(label);
      sendCommand(c);
    }
  }

  void setValue(bool s) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(s ? 1 : 0);
    sendCommand(c);
  }

  bool call() override {
    sendCommand(Command(Command::SET_INT_VALUE, getInternalId()));
    return true;
  }
  bool call(bool t) override { return call(); }
  bool call(int i) override { return call(); }
  bool call(const std::string & s) override { return call(); }

  int linkedView = 0;

protected:
  void create() override {
    Command c(Command::CREATE_BUTTON, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    sendCommand(c);
  }

 private:
  std::string label;
};

#endif
