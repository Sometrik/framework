#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Element.h>
#include <Message.h>

class Button : public Element {
 public:
  Button(const std::string & _label) : label(_label) { }

  void initialize(FWPlatformBase * _platform) override {
    FWViewBase::initialize(_platform);
    Message m(Message::CREATE_BUTTON, getId(), getParentId());
    m.setTextValue(label);
    sendMessage(m);
  }

 private:
  std::string label;
};

#endif
