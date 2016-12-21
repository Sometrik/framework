#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include <Element.h>
#include <Command.h>

class TextLabel : public Element {
 public:
 TextLabel(const std::string & _text, unsigned int _flags = 0) : text(_text), flags(_flags) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_TEXTLABEL, getParentInternalId(), getInternalId());
    c.setTextValue(text);
    c.setFlags(getFlags());
    sendCommand(c);
  }

 private:
  std::string text;
};

#endif
