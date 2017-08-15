#ifndef _SIMPLELISTVIEW_H_
#define _SIMPLELISTVIEW_H_

#include <Element.h>

#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

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

  virtual void reshapeTable(int size) {
     Command c(Command::RESHAPE_TABLE, getInternalId());
     c.setValue(size);
     sendCommand(c);
   }

   void reshapeSheet(int sheet, int size) {
     Command c(Command::RESHAPE_SHEET, getInternalId());
     c.setSheet(sheet);
     c.setValue(size);
     sendCommand(c);
   }

  void onValueEvent(ValueEvent & ev) override {
    selected_sheet = ev.getValue2();
    selected_row = ev.getValue();

    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
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
  int selected_sheet = 0, selected_row = 0;
};

#endif
