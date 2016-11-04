#ifndef _LINEARLAYOUT_H_
#define _LINEARLAYOUT_H_

#include <Element.h>
#include <Message.h>

#define FW_VERTICAL	1
#define FW_HORIZONTAL	2

class LinearLayout : public Element {
 public:
  LinearLayout(int _direction) : direction(_direction) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Message m(Message::CREATE_LINEAR_LAYOUT, getInternalId(), getParentInternalId());
    m.setValue(direction);
    sendMessage(m);
  }
			       
 private:
  int direction = FW_VERTICAL;
};

#endif
