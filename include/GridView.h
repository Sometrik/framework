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

  bool isA(const std::string & className) override {
    if (className == "GridView") return true;
    return Element::isA(className);
  }

  void addColumn(const std::string & name) {
    columns.push_back(GridViewColumn(name));
    if (isInitialized()) {
      initializeColumn(columns.back());
    }
  }

 protected:
  void initializeColumn(const GridViewColumn & col) {
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setTextValue(col.getName());
    sendCommand(c);    
  }
  
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_GRIDVIEW, getParentInternalId(), getInternalId()));
    for (auto & c : columns) {
      initializeColumn(c);
    }
  }

 private:
  std::vector<GridViewColumn> columns;
};

#endif
