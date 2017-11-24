#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Element.h>

#include <ValueEvent.h>
#include <Command.h>
#include <CommandEvent.h>

class Button : public Element {
 public:
 Button(const std::string & _label, int _id = 0, unsigned int _flags = 0)
   : Element(_id, _flags), label(_label) { }

  bool isA(const std::string & className) const override {
    if (className == "Button") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }
  
  void text(const std::string & l) override {
    if (l != label) {
      label = l;
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setTextValue(label);
      sendCommand(c);
    }
  }

  void setValue(bool s) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(s ? 1 : 0);
    sendCommand(c);
  }

  bool call() override {
    sendCommand(Command(Command::SET_INT_VALUE, getInternalId()));
    return true;
  }
  bool call(bool t) override { return call(); }
  bool call(int i) override { return call(); }
  bool call(const std::string & s) override { return call(); }

  int linkedView = 0;
  std::string getLabel() { return label; }

protected:
  void create() override {
    Command c(Command::CREATE_BUTTON, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    sendCommand(c);
  }
  std::string label;
};

#endif
