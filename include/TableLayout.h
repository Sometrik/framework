#ifndef _TABLELAYOUT_H_
#define _TABLELAYOUT_H_

#include <Element.h>
#include <Command.h>

class TableLayout : public Element {
 public:
  TableLayout(int _num_columns) : num_columns(_num_columns) { }

  bool isA(const std::string & className) const override {
    if (className == "TableLayout") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_TABLE_LAYOUT, getParentInternalId(), getInternalId());
    c.setValue(num_columns);
    sendCommand(c);
  }

 private:
  int num_columns;
};

#endif
