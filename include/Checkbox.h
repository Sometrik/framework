#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <InputElement.h>

#include <Command.h>
#include <FWPlatform.h>

class Checkbox : public InputElement {
 public:
  Checkbox(const std::string & _label) : label(_label) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_CHECKBOX, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue() != 0;
    notify(value);
  }

  bool isChecked() { return value; }

 private:
  bool value = false;
  std::string label;
};

#endif
