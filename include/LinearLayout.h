#ifndef _LINEARLAYOUT_H_
#define _LINEARLAYOUT_H_

#include <Element.h>
#include <Command.h>

#define FW_VERTICAL	1
#define FW_HORIZONTAL	2

class LinearLayout : public Element {
 public:
  LinearLayout(int _direction, int _id) : Element(_id), direction(_direction) { }

  bool isA(const std::string & className) override {
    if (className == "LinearLayout") return true;
    return Element::isA(className);
  }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_LINEAR_LAYOUT, getParentInternalId(), getInternalId());
    c.setValue(direction);
    sendCommand(c);
  }
			       
 private:
  int direction = FW_VERTICAL;
};

#endif
