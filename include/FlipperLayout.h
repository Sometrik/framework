#ifndef _FLIPPERLAYOUT_H_
#define _FLIPPERLAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class FlipperLayout : public Element {
 public:
  FlipperLayout(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "FlipperLayout") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void setVisibleView(int position) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(position);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_FLIPPER_LAYOUT, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
