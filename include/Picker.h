#ifndef _PICKER_H_
#define _PICKER_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>

class Picker : public Element {
 public:
  Picker() { }
  
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_PICKER, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
    for (auto & o : options) {
      initializeOption(o);
    }
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
  }

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
