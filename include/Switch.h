#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <InputElement.h>

#include <Command.h>
#include <FWPlatform.h>

class Switch : public InputElement {
 public:
  Switch(const std::string & _on_label, const std::string & _off_label)
    : on_label(_on_label), off_label(_off_label) { }

  bool isA(const std::string & className) const override {
    if (className == "Switch") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue() != 0;
    notify(value);
  }

  bool getValue() const { return value; }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_SWITCH, getParentInternalId(), getInternalId());
    c.setTextValue(on_label);
    c.setTextValue2(off_label);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  bool value = false;
  std::string on_label, off_label;
};

#endif
