#ifndef _PANEL_H_
#define _PANEL_H_

#include <Element.h>
#include <Command.h>

class Panel : public Element {
 public:
  Panel(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "Panel") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_PANEL, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
