#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include <InputElement.h>
#include <Command.h>
#include <ValueEvent.h>

class TextField : public InputElement {
 public:
 TextField(unsigned int _flags = 0) : InputElement(0, _flags) {  }

  bool isA(const std::string & className) override {
    if (className == "TextField") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
  }

  void clear() {
    Command c(Command::CLEAR, getInternalId());
    sendCommand(c);
  }

  bool call(bool t) override { return call(t ? "true" : "false"); }
  bool call(const std::string & s) override {
    value = s;
    Command c(Command::SET_TEXT_VALUE, getParentInternalId(), getInternalId());
    c.setTextValue(value);
    sendCommand(c);
    return true;
  }

  const std::string & getValue() const { return value; }

 protected:

  void create() override {
    Command c(Command::CREATE_TEXTFIELD, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
  }

 private:
  std::string value;
};

#endif
