#ifndef _NAVIGATIONDRAWER_H_
#define _NAVIGATIONDRAWER_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>
#include <VisibilityEvent.h>

class NavigationDrawer : public Element {
 public:
  NavigationDrawer(int _id = 0, unsigned int _flags = 0) : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "NavigationDrawer") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void hide() override {
    Command c(Command::SET_VISIBILITY, getInternalId());
    c.setValue(0);
    sendCommand(c);
  }

  bool isVisible() const override {
    return is_visible;
  }

 protected:

  void create() override {
    Command c(Command::CREATE_NAVIGATIONVIEW, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
