#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <Element.h>

#include <Command.h>
#include <CommandEvent.h>
#include <FWPlatform.h>

class Checkbox : public Element {
 public:
  Checkbox() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_CHECKBOX, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

  bool isChecked() { return checked; }

 private:
  bool checked = false;
};

#endif
