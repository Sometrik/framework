#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <Element.h>

#include <Command.h>
#include <CommandEvent.h>
#include <FWPlatform.h>

class Checkbox : public Element {
 public:
  Checkbox(const std::string & _label) : label(_label){ }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_CHECKBOX, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    if (ev.getValue() == 0){
      checked = false;
    } else {
      checked = true;
    }
  }

  bool isChecked() { return checked; }

 private:
  bool checked = false;
  std::string label;
};

#endif
