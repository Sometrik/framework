#ifndef _SCROLLLAYOUT_H_
#define _SCROLLLAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class ScrollLayout : public Element {
 public:
  ScrollLayout(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "ScrollLayout") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void refresh() override {
    for (auto & child : getChildren()) {
      child->refresh();
    }
  }

 protected:
  void create() override {
    Command c(Command::CREATE_SCROLL_LAYOUT, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
