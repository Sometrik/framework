#ifndef _SCROLLLAYOUT_H_
#define _SCROLLLAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class ScrollLayout : public Element {
 public:
  ScrollLayout(int _id = 0) : Element(_id) {
#ifndef __ANDROID__
    style("width", "match-parent");
    style("height", "match-parent");
#endif
  }

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

  void setScrollPosition(int pos) {
    Command c(Command::SET_INT_VALUE, getParentInternalId(), getInternalId());
    c.setValue(pos);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_SCROLL_LAYOUT, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
