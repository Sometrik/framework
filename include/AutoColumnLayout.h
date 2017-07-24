#ifndef _AUTOCOLUMNLAYOUT_H_
#define _AUTOCOLUMNLAYOUT_H_

#include <Element.h>
#include <Command.h>

class AutoColumnLayout : public Element {
 public:
  AutoColumnLayout() { }

  bool isA(const std::string & className) const override {
    if (className == "AutoColumnLayout") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    sendCommand(Command(Command::CREATE_AUTO_COLUMN_LAYOUT, getParentInternalId(), getInternalId()));
  }
};

#endif
