#ifndef _LABEL_H_
#define _LABEL_H_

#include <Element.h>
#include <Message.h>

class Label : public Element {
 public:
  Label(const std::string & _text) : text(_text) { }

  void initialize(FWPlatformBase * _platform) override {
    FWViewBase::initialize(_platform);
    Message m(Message::CREATE_LABEL, getId(), getParentId());
    m.setTextValue(text);
    sendMessage(m);
  }

 private:
  std::string text;
};

#endif
