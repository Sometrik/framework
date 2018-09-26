#ifndef _NAVIGATIONDRAWER_H_
#define _NAVIGATIONDRAWER_H_

#include <Element.h>
#include <Command.h>
// #include <ValueEvent.h>

class NavigationDrawer : public Element {
 public:
  NavigationDrawer(int _id = 0, unsigned int _flags = 0) : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "NavigationDrawer") return true;
    return Element::isA(className);
  }

#if 0
  void onValueEvent(ValueEvent & ev) override {
    is_visible = ev.getValue() != 0;
    if (is_visible && !is_content_initialized) {
      initializeContent();
    }
    notify(ev.getValue());
    ev.setHandled(true);
  }
#endif

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
