#ifndef _PICKER_H_
#define _PICKER_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>

class Picker : public Element {
 public:
  Picker(int id = 0) : Element(id) { }

  bool isA(const std::string & className) const override {
    if (className == "Picker") return true;
    return Element::isA(className);
  }

  void addOption(int id, const std::string & name) {
    if (options.empty()){
      value = id;
    }
    options.push_back(std::pair<int, std::string>(id, name));

    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(id);
    c.setTextValue(name);
    sendCommand(c);
  }
  void setSelection(int position) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(position);
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue();
    notify(value);
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  int getValue() {
    return value;
  }

  void clear() {
    Command c(Command::CLEAR, getInternalId());
    sendCommand(c);
  }

  bool call() override { return call(value); }
  bool call(bool t) override { return call(t ? value : 0); }
  bool call(int i) override {
    value = i;
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
    return true;
  }
  bool call(const std::string & s) override {
    return true;
  }

 protected:
  void create() override {
    Command c(Command::CREATE_PICKER, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:
  int value = 0;
  std::vector<std::pair<int, std::string> > options;
};

#endif
