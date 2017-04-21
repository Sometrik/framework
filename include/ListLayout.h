#ifndef _LISTLAYOUT_H_
#define _LISTLAYOUT_H_

#include <UIElement.h>

#include <Command.h>
#include <CommandEvent.h>

class ListLayout : public Element {
 public:
  ListLayout() { }

  bool isA(const std::string & className) override {
    if (className == "ListLayout") return true;
    return Element::isA(className);
  }

  void addColumn(const char * name) {
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setTextValue(name);
    sendCommand(c);
  }

  void addData(int row, int column, const char * value) {
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
    sendCommand(Command(Command::CREATE_GRIDVIEW, getParentInternalId(), getInternalId()));
  }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_LISTVIEW, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
};

#endif
