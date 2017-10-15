#ifndef _FRAMEVIEW_H_
#define _FRAMEVIEW_H_

#include <FWViewBase.h>
#include <Command.h>

class FrameView : public FWViewBase {
 public:
  FrameView(int _id = 0) : FWViewBase(_id) { }
  FrameView(const std::string & label, int _id = 0) : FWViewBase(label, _id) { }

  bool isA(const std::string & className) const override {
    if (className == "FrameView") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_FRAMEVIEW, getParentInternalId(), getInternalId());
    c.setTextValue(getTitle());
    sendCommand(c);
  }
};

#endif
