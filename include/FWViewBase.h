#ifndef _FWVIEW_H_
#define _FWVIEW_H_

#include <Element.h>
#include <Command.h>

class FWViewBase : public Element {
 public:
  FWViewBase(int _id) : Element(_id) { }

  void show() override {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(1);
    sendCommand(c);
  }
};

#endif
