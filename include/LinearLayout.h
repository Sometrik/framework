#ifndef _LINEARLAYOUT_H_
#define _LINEARLAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

#define FW_VERTICAL	1
#define FW_HORIZONTAL	2

class LinearLayout : public Element {
 public:
  LinearLayout(int _direction = FW_VERTICAL, int _id = 0) : Element(_id), direction(_direction) { }

  bool isA(const std::string & className) const override {
    if (className == "LinearLayout") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void setDirection(int dir) { direction = dir; }

 protected:
  void create() override {
    Command c(Command::CREATE_LINEAR_LAYOUT, getParentInternalId(), getInternalId());
    c.setValue(direction);
    sendCommand(c);
  }

 private:
  int direction = FW_VERTICAL;
};

#endif
