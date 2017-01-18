#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <InputElement.h>

#include <Command.h>
#include <FWPlatform.h>

class Switch : public InputElement {
 public:
  Switch(const std::string & _label) : label(_label) { }
  Switch(const std::string & _label, const std::string & _label2) : label(_label), label2(_label2) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_SWITCH, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    c.setTextValue(label2);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
    initializeChildren();
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue() != 0;
    notify(value);
  }

  bool getValue() const { return value; }

 private:
  bool value = false;
  std::string label, label2;
};

#endif
