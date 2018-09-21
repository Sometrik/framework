#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <InputElement.h>
#include <CommandEvent.h>

class Switch : public InputElement {
 public:
  Switch(int _id = 0) : InputElement(_id) { }

  Switch(int _id, const std::string & _on_label, const std::string & _off_label)
    : InputElement(_id), on_label(_on_label), off_label(_off_label) { }

  Switch(const std::string & _on_label, const std::string & _off_label)
    : on_label(_on_label), off_label(_off_label) { }

  bool isA(const std::string & className) const override {
    if (className == "Switch") return true;
    return InputElement::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue() != 0;
    notify(value);
    ev.setHandled(true);

    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  bool getValue() const { return value; }

 protected:
  void create() override {
    Command c(Command::CREATE_SWITCH, getParentInternalId(), getInternalId());
    c.setTextValue(on_label);
    c.setTextValue2(off_label);
    sendCommand(c);
  }

 private:
  bool value = false;
  std::string on_label, off_label;
};

#endif
