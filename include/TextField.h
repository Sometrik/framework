#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include <InputElement.h>
#include <Command.h>
#include <ValueEvent.h>

class TextField : public InputElement {
 public:
 TextField(unsigned int _flags = 0) : InputElement(0, _flags) {  }

  bool isA(const std::string & className) const override {
    if (className == "TextField") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
  }

  void clear() {
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }

  bool call(bool t) override { return call(t ? "true" : "false"); }
  bool call(const std::string & s) override {
    value = s;
    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(value);
    sendCommand(c);
    return true;
  }

  const std::string & getValue() const { return value; }

 protected:
  void create() override {
    Command c(Command::CREATE_TEXTFIELD, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:
  std::string value;
};

#endif
