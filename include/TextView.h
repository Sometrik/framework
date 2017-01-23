#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include <InputElement.h>
#include <Command.h>
#include <ValueEvent.h>

class TextView : public InputElement {
 public:
 TextView() { }

  bool isA(const std::string & className) override {
    if (className == "TextView") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
  }

  bool call(bool t) override { return call(t ? "true" : "false"); }
  bool call(const std::string & s) override {
    value = s;
    Command c(Command::SET_TEXT_VALUE, getParentInternalId(), getInternalId());
    c.setTextValue(value);
    sendCommand(c);
    return true;
  }

  const std::string & getValue() { return value; }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_TEXTVIEW, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  std::string value;
};

#endif
