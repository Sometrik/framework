#ifndef _TABLELAYOUT_H_
#define _TABLELAYOUT_H_

#include <Element.h>
#include <Command.h>

class TableLayout : public Element {
 public:
  TableLayout(int _direction) : direction(_direction) { }

  bool isA(const std::string & className) override {
    if (className == "TableLayout") return true;
    return Element::isA(className);
  }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_TABLE_LAYOUT, getParentInternalId(), getInternalId()));
  }
};

#endif
