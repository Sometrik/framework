#ifndef _FRAMELAYOUT_H_
#define _FRAMELAYOUT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class FrameLayout : public Element {
 public:
  FrameLayout(int _id = 0) : Element(_id) {
#ifndef __ANDROID__
    style("width", "match-parent");
    style("height", "match-parent");
#endif
  }

  bool isA(const std::string & className) const override {
    if (className == "FrameLayout") return true;
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
    Command c(Command::CREATE_FRAME_LAYOUT, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
