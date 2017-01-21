#ifndef _PICKER_H_
#define _PICKER_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>

class Picker : public Element {
 public:
  Picker() { }

  bool isA(const std::string & className) override {
    if (className == "Picker") return true;
    return Element::isA(className);
  }

  void addOption(int id, const std::string & name) {
    if (options.empty()){
      value = id;
    }
    options.push_back(std::pair<int, std::string>(id, name));
    if (isInitialized()) {
      initializeOption(options.back());
    }
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getValue();
    notify(value);
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
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_PICKER, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
    for (auto & o : options) {
      initializeOption(o);
    }
  }

  void initializeOption(const std::pair<int, std::string> & o) {
    Command c(Command::ADD_OPTION, getInternalId(), 0);
    c.setValue(o.first);
    c.setTextValue(o.second);
    sendCommand(c);
  }

 private:
  int value = 0;
  std::vector<std::pair<int, std::string> > options;
};

#endif
