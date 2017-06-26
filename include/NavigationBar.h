#ifndef _NAVIGATIONBAR_H_
#define _NAVIGATIONBAR_H_

#include <UIElement.h>

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
    CommandEvent ev2(ev.getTimestamp(), getId());
    ev2.dispatch(*this);
  }

  void addOption(int optionId, std::string imageFileName){
    Command c(Command::ADD_OPTION, getInternalId());
    c.setTextValue(imageFileName);
    c.setValue(optionId);
    sendCommand(c);
  }

protected:
  void create() override {
    Command c(Command::CREATE_NAVIGATIONBAR, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

};

#endif
