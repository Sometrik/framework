#ifndef _FWVIEW_H_
#define _FWVIEW_H_

#include <Element.h>

class FWViewBase : public Element {
 public:
  FWViewBase() { }

  void show() override {
    sendMessage(Message(Message::SHOW_VIEW, getInternalId()));
  }
};

#endif
