#ifndef _GRIDVIEW_H_
#define _GRIDVIEW_H_

#include <Element.h>
#include <Command.h>

class GridViewColumn {
 public:
  GridViewColumn() { }
 GridViewColumn(const std::string & _name) : name(_name) { }

  const std::string & getName() const { return name; }
  
 private:
  std::string name;
};

class GridView : public Element {
 public:
  GridView() { }

  bool isA(const std::string & className) const override {
    if (className == "GridView") return true;
    return Element::isA(className);
  }

  void addColumn(const std::string & name) {
    columns.push_back(GridViewColumn(name));
    auto & col = columns.back();
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setTextValue(col.getName());
    sendCommand(c);    
  }

  void setValue(int row, int column, const std::string & value) {
    Command c(Command::SET_TEXT_DATA, getInternalId());
    c.setRow(row);
    c.setColumn(column);
    c.setTextValue(value);
    sendCommand(c);    
  }

  void flush() {
    sendCommand(Command(Command::FLUSH_VIEW, getInternalId()));    
  }

 protected: 
  void create() override {
    sendCommand(Command(Command::CREATE_GRIDVIEW, getParentInternalId(), getInternalId()));
  }

 private:
  std::vector<GridViewColumn> columns;
};

#endif
