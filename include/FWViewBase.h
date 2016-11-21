#ifndef _FWVIEW_H_
#define _FWVIEW_H_

#include <Element.h>
#include <Command.h>

class FWViewBase : public Element {
 public:
  FWViewBase(int _id) : Element(_id) { }

  void show() override {
    sendMessage(Command(Command::SHOW_VIEW, getInternalId()));
  }
};

#endif
