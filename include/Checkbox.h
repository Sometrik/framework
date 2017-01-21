#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <InputElement.h>

#include <Command.h>
#include <FWPlatform.h>

class Checkbox : public InputElement {
 public:
  Checkbox(const std::string & _label) : label(_label) { }

  bool isA(const std::string & className) override {
    if (className == "Checkbox") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue() != 0;
    notify(value);
  }

  bool call(bool t) override {
    value = t;
    Command c(Command::SET_INT_VALUE, getParentInternalId(), getInternalId());
    c.setValue(value);
    sendCommand(c);
    return true;
  }
  bool call(const std::string & s) override {
    return call(!s.empty() && s != "0" && s != "false");
  }

  bool getValue() const { return value; }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_CHECKBOX, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  bool value = false;
  std::string label;
};

#endif
