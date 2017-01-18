#ifndef _RADIOGROUP_H_
#define _RADIOGROUP_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>

class RadioGroup : public Element {
 public:
  RadioGroup() { }
  
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_RADIO_GROUP, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
    for (auto & o : options) {
      initializeOption(o);
    }
    initializeChildren();
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
