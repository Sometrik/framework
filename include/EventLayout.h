#ifndef _EVENTLAYOUT_H_
#define _EVENTLAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class EventLayout : public Element {
 public:
  EventLayout(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "EventLayout") return true;
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
    Command c(Command::CREATE_EVENT_LAYOUT, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
