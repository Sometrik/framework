#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include <Element.h>
#include <Message.h>

class TextField : public Element {
 public:
 TextField() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendMessage(Message(Message::CREATE_TEXTFIELD, getId(), getParentId()));
  }

  const std::string & getValue() { return value; }
			       
 private:
  std::string value;
};

#endif
