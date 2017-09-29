#ifndef _RELATIVELAYOUT_H_
#define _RELATIVELAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class RelativeLayout : public Element {
 public:
  RelativeLayout(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "RelativeLayout") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
     notify();
     CommandEvent ev2(getId());
     ev2.dispatch(*this);
   }

  void addRule(int childId, std::string rule) {
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(childId);
    c.setTextValue(rule);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_RELATIVE_LAYOUT, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

};

#endif
