#ifndef _TABLELAYOUT_H_
#define _TABLELAYOUT_H_

#include <Element.h>
#include <Command.h>

class TableLayout : public Element {
 public:
  TableLayout(int _direction) : direction(_direction) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_TABLE_LAYOUT, getParentInternalId(), getInternalId()));
    initializeChildren();
  }
};

#endif
