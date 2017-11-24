#ifndef _NAVIGATIONBARITEM_H_
#define _NAVIGATIONBARITEM_H_

#include <Button.h>

class NavigationBarItem : public Button {
 public:
 NavigationBarItem(const std::string & _label, int _id = 0)
   : Button(_label, _id) { }

  bool isA(const std::string & className) const override {
    if (className == "NavigationBarItem") return true;
    return Button::isA(className);
  }

protected:
  void create() override {
    Command c(Command::CREATE_NAVIGATIONBAR_ITEM, getParentInternalId(), getInternalId());
    c.setTextValue(getLabel());
    sendCommand(c);
  }
};

#endif
