#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <InputElement.h>

#include <Command.h>

class Checkbox : public InputElement {
 public:
  Checkbox(const std::string & _label) : label(_label) { }

  bool isA(const std::string & className) const override {
    if (className == "Checkbox") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue() != 0;
    notify(value);
    ev.setHandled(true);
  }

  bool call(bool t) override {
    value = t;
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
    return true;
  }
  bool call(const std::string & s) override {
    return call(!s.empty() && s != "0" && s != "false");
  }

  bool getValue() const { return value; }

 protected:
  void create() override {
    Command c(Command::CREATE_CHECKBOX, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    sendCommand(c);
  }

 private:
  bool value = false;
  std::string label;
};

#endif
