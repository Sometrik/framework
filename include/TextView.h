#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include <InputElement.h>
#include <Command.h>
#include <ValueEvent.h>

class TextView : public InputElement {
 public:
 TextView() { }

  bool isA(const std::string & className) const override {
    if (className == "TextView") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
  }

  bool call(bool t) override { return call(t ? "true" : "false"); }
  bool call(const std::string & s) override {
    setValue(s);
    return true;
  }

  const std::string & getValue() { return value; }

  void setValue(const std::string & s) {
    if (s != value) {
      value = s;
      
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setTextValue(value);
      sendCommand(c);    
    }
  }

 protected:
  void create() override {
    Command c(Command::CREATE_TEXTVIEW, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
  
 private:
  std::string value;
};

#endif
