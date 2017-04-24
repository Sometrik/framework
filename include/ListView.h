#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include <UIElement.h>

#include <Command.h>
#include <CommandEvent.h>

class ListView : public Element {
 public:
  ListView() { }

  bool isA(const std::string & className) override {
    if (className == "ListView") return true;
    return Element::isA(className);
  }

  void addColumn(const std::string & name) {
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setTextValue(name);
    sendCommand(c);
  }

  void setValue(int row, int column, const std::string & value) {
    Command c(Command::SET_TEXT_DATA, getInternalId());
    c.setRow(row);
    c.setColumn(column);
    c.setTextValue(value);
    sendCommand(c);
  }

  void addHeaderBar(int row, const char * text) {
    Command c(Command::ADD_COLUMN, getInternalId(), text);
    c.setValue(row);
    sendCommand(c);
  }

  void flush() {
    sendCommand(Command(Command::FLUSH_VIEW, getInternalId()));
  }

  void clear() {
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }

 protected: 
  void create() override {
    Command c(Command::CREATE_LISTVIEW, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }
};

#endif
