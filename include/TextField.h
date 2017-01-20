#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include <InputElement.h>
#include <Command.h>
#include <ValueEvent.h>

class TextField : public InputElement {
 public:
 TextField() { }

  bool isA(const std::string & className) override {
    if (className == "TextField") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
  }

  void call(bool t) override { call(t ? "true" : "false"); }
  void call(const std::string & s) override {
    value = s;
    Command c(Command::SET_TEXT_VALUE, getParentInternalId(), getInternalId());
    c.setTextValue(value);
    sendCommand(c);
  }

  const std::string & getValue() { return value; }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_TEXTFIELD, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  std::string value;
};

#endif
