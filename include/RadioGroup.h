#ifndef _RADIOGROUP_H_
#define _RADIOGROUP_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>

class RadioGroup : public Element {
 public:
  RadioGroup() { }

  bool isA(const std::string & className) const override {
    if (className == "RadioGroup") return true;
    return Element::isA(className);
  }

  void addOption(int id, const std::string & name) {
    if (options.empty()) {
      value = id;
    }
    options.push_back(std::pair<int, std::string>(id, name));

    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(id);
    c.setTextValue(name);
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue();
    notify(value);
    ev.setHandled(true);
  }

  void call() override { call(value); }
  void call(bool t) override { call(t ? value : 0); }
  void call(int i) override {
    value = i;
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
  }
  void call(const std::string & s) override { }

 protected:
  void create() override {
    Command c(Command::CREATE_RADIO_GROUP, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  int value = 0;
  std::vector<std::pair<int, std::string> > options;
};

#endif
