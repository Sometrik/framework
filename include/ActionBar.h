#ifndef _ACTIONBAR_H_
#define _ACTIONBAR_H_

#include <InputElement.h>
#include <CommandEvent.h>
#include <Command.h>
#include <ValueEvent.h>

class ActionBar : public Element {
 public:
  ActionBar() { }

  bool isA(const std::string & className) const override {
    if (className == "ActionBar") return true;
    return Element::isA(className);
  }

  bool isVisible() const override {
    return getParent() && getParent()->isVisible();
  }

  void addOption(int id, const std::string & name) {
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(id);
    c.setTextValue(name);
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void text(const std::string & s) override {
    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(s);
    sendCommand(c);
  }

  void setTitle(const std::string & title, const std::string & subtitle) {
    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(title);
    c.setTextValue2(subtitle);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_ACTIONBAR, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 protected:
};

#endif
