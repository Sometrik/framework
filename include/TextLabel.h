#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include <Element.h>
#include <Command.h>

class TextLabel : public Element {
 public:
  TextLabel(const std::string & _text) : text(_text) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_TEXTLABEL, getParentInternalId(), getInternalId());
    c.setTextValue(text);
    sendCommand(c);
  }

 private:
  std::string text;
};

#endif
