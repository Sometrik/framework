#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include <Element.h>
#include <Message.h>

class TextLabel : public Element {
 public:
  TextLabel(const std::string & _text) : text(_text) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Message m(Message::CREATE_TEXTLABEL, getParentInternalId(), getInternalId());
    m.setTextValue(text);
    sendMessage(m);
  }

 private:
  std::string text;
};

#endif
