#ifndef _PICKER_H_
#define _PICKER_H_

#include <Element.h>
#include <Message.h>

class Picker : public Element {
 public:
  Picker(const std::string & _label) : label(_label) { }
  
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Message m(Message::CREATE_PICKER, getId(), getParentId());
    m.setTextValue(label);
    sendMessage(m);
    for (auto & o : options) {
      initializeOption(o);
    }
  }

  void addOption(int id, const std::string & name) {
    options.push_back(std::pair<int, std::string>(id, name));
    if (isInitialized()) {
      initializeOption(options.back());
    }
  }

 protected:
  void initializeOption(const std::pair<int, std::string> & o) {
    Message m(Message::ADD_OPTION, getId(), getParentId());
    m.setValue(o.first);
    m.setTextValue(o.second);
    sendMessage(m);
  }

 private:
  int value;
  std::vector<std::pair<int, std::string> > options;
};

#endif
