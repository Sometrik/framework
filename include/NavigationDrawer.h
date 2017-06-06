#ifndef _NAVIGATIONDRAWER_H_
#define _NAVIGATIONDRAWER_H_

#include <Element.h>
#include <Command.h>

class NavigationDrawer : public Element {
 public:
  NavigationDrawer() : Element() { }

  bool isA(const std::string & className) const override {
    if (className == "NavigationDrawer") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_NAVIGATIONVIEW, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
			       
 private:
};

#endif
