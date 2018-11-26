#ifndef _ACTIONBAR_H_
#define _ACTIONBAR_H_

#include <Element.h>
#include <CommandEvent.h>
#include <Command.h>
#include <ValueEvent.h>

class ActionBar : public Element {
 public:
  ActionBar(bool _has_back_button = false) : has_back_button(_has_back_button) { }
  ActionBar(const std::string & _title, bool _has_back_button = false)
    : title(_title), has_back_button(_has_back_button) { }
  ActionBar(const std::string & _title, const std::string & _subtitle, bool _has_back_button = false)
    : title(_title), subtitle(_subtitle), has_back_button(_has_back_button) { }

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

  bool hasBackButton() const { return has_back_button; }

 protected:
  void create() override {
    Command c(Command::CREATE_ACTIONBAR, getParentInternalId(), getInternalId());
    c.setTextValue(title);
    c.setTextValue2(subtitle);
    c.setFlags(has_back_button ? 1 : 0);
    sendCommand(c);
  }

  std::string title, subtitle;
  bool has_back_button;
};

#endif
