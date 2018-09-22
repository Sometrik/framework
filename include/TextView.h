#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include <InputElement.h>
#include <Command.h>
#include <ValueEvent.h>
#include <CommandEvent.h>

class TextView : public InputElement {
 public:
 TextView(int _id = 0, unsigned int _flags = 0) : InputElement(_id, _flags) {
    style("width", "match-parent");
 }
 TextView(const std::string & _value, int _id = 0, unsigned int _flags = 0) : InputElement(_id, _flags), value(_value) {
    style("width", "match-parent");
 }

  bool isA(const std::string & className) const override {
    if (className == "TextView") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  bool call(bool t) override { return call(t ? "true" : "false"); }
  bool call(const std::string & s) override {
    text(s);
    return true;
  }

  void text(const std::string & _value) override {
    value = _value;

    if (isInitialized()) {
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setTextValue(value);
      sendCommand(c);
    }
  }

  void focus(bool enabled) override {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(enabled ? 1 : 0);
    sendCommand(c);
  }

  const std::string & getValue() const { return value; }

 protected:
  void create() override {
    Command c(Command::CREATE_TEXTVIEW, getParentInternalId(), getInternalId());
    c.setTextValue(value);
    sendCommand(c);
  }
  
 private:
  std::string value;
};

#endif
