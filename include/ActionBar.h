#ifndef _ACTIONBAR_H_
#define _ACTIONBAR_H_

#include <InputElement.h>

#include <Command.h>
#include <FWPlatform.h>

class ActionBar : public Element {
 public:
  ActionBar() { }
  ActionBar(const char * _title) : title(_title) { }

  bool isA(const std::string & className) const override {
    if (className == "ActionBar") return true;
    return Element::isA(className);
  }

  void addOption(int id, const std::string & name) {
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(id);
    c.setTextValue(name);
    sendCommand(c);
  }

  void setTitle(const std::string & _title){
    title = _title;
    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(title);
    sendCommand(c);
  }

 protected:

  void create() override {
    Command c(Command::CREATE_ACTIONBAR, getParentInternalId(), getInternalId());
    c.setTextValue(title);
    sendCommand(c);
  }

 private:
  std::string title;
};

#endif
