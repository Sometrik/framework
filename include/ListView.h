#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include <Element.h>

#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class ListView : public Element {
 public:
  enum ColumnType { TEXT = 1, NUMERIC, TIMESTAMP, ICON };
 ListView(int _id = 0, unsigned int _flags = 0) : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "ListView") return true;
    return Element::isA(className);
  }

  void addSheet(const std::string & name) {
    Command c(Command::ADD_SHEET, getInternalId());
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

  // Used to expand a sheet
  void setValue(int value) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
  }

  void flush() {
    sendCommand(Command(Command::FLUSH_VIEW, getInternalId()));
  }

  void onValueEvent(ValueEvent & ev) override {
    selected_sheet = ev.getValue();
    selected_row = ev.getValue2();
    
    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

 protected: 
  void create() override {
    Command c(Command::CREATE_LISTVIEW, getParentInternalId(), getInternalId());
    c.setFlags(getFlags());
    sendCommand(c);
  }

  int selected_sheet = 0, selected_row = 0;
};

#endif
