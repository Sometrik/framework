#ifndef _SIMPLELISTVIEW_H_
#define _SIMPLELISTVIEW_H_

#include <Element.h>

#include <Command.h>
#include <CommandEvent.h>

class SimpleListView : public Element {
 public:
 SimpleListView(int _id = 0, unsigned int _flags = 0) : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "SimpleListView") return true;
    return Element::isA(className);
  }

  void setSheet(int sheet, const std::string & name) {
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(sheet);
    c.setTextValue(name);
    sendCommand(c);
  }

  // Used to expand a sheet
  void setValue(int value) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
  }

 protected: 
  void create() override {
    Command c(Command::CREATE_SIMPLELISTVIEW, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    c.setFlags(getFlags());
    sendCommand(c);
  }
};

#endif