#ifndef _GRIDVIEW_H_
#define _GRIDVIEW_H_

#include <Element.h>
#include <Command.h>

class GridView : public Element {
 public:
  GridView() { }

  bool isA(const std::string & className) override {
    if (className == "GridView") return true;
    return Element::isA(className);
  }

  void addColumn(const std::string & name) {
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setTextValue(name);
    sendCommand(c);    
  }
  
 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_GRIDVIEW, getParentInternalId(), getInternalId()));
  }
};

#endif
