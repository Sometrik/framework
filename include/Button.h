#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <TouchEvent.h>
#include <FWPlatform.h>

class Button : public Element {
 public:
  Button(const std::string & _label) : label(_label) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_BUTTON, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    sendCommand(c);
  }

  void onTouchEvent(TouchEvent & ev) override {
    getPlatform().getLogger().println("Button received event");
    if (ev.getType() == TouchEvent::ACTION_CLICK) {
      getPlatform().getLogger().println("Button received event was handled");
      CommandEvent ev2(ev.getTimestamp(), getId());
      ev2.dispatch(*this);
    }
  }

 private:
  std::string label;
};

#endif
