#ifndef _NAVIGATIONBAR_H_
#define _NAVIGATIONBAR_H_

#include <Element.h>

#include <ValueEvent.h>
#include <Command.h>
#include <CommandEvent.h>

class NavigationBar : public Element {
 public:
  NavigationBar(int _id = 0, unsigned int _flags = 0)
   : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "NavigationBar") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

protected:
  void create() override {
    Command c(Command::CREATE_NAVIGATIONBAR, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
