#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include <UIElement.h>

#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class ListView : public Element {
 public:
  enum ColumnType { TEXT = 1, NUMERIC, TIMESTAMP, ICON };
  ListView() { }

  bool isA(const std::string & className) override {
    if (className == "ListView") return true;
    return Element::isA(className);
  }

  void addSheet(const std::string & name) {
    Command c(Command::ADD_SHEET, getInternalId());
    c.setTextValue(name);
    sendCommand(c);
  }

  void addColumn(const std::string & name, ColumnType type = TEXT) {
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setTextValue(name);
    c.setValue(int(type));
    sendCommand(c);
  }

  void setValue(const std::string & value, int row, int column, int sheet = 0) {
    Command c(Command::SET_TEXT_DATA, getInternalId());
    c.setRow(row);
    c.setColumn(column);
    c.setSheet(sheet);
    c.setTextValue(value);
    sendCommand(c);
  }

  //Used to expand a sheet
  void setValue(int value) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
  }

  void flush() {
    sendCommand(Command(Command::FLUSH_VIEW, getInternalId()));
  }

  void clear() {
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }

  void onValueEvent(ValueEvent & ev) override {
    selected_sheet = ev.getValue();
    selected_row = ev.getValue2();
    
    notify();
    CommandEvent ev2(ev.getTimestamp(), getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
  }

 protected: 
  void create() override {
    Command c(Command::CREATE_LISTVIEW, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

  int selected_sheet = 0, selected_row = 0;
};

#endif
