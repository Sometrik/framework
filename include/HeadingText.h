#ifndef _HEADINGTEXT_H_
#define _HEADINGTEXT_H_

#include <Element.h>
#include <Command.h>

class HeadingText : public Element {
 public:
  HeadingText(const std::string & _text, unsigned int _flags = 0) : Element(0, _flags), text(_text) { }

  bool isA(const std::string & className) const override {
    if (className == "HeadingText") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_HEADING_TEXT, getParentInternalId(), getInternalId());
    c.setTextValue(text);
    c.setFlags(getFlags());
    sendCommand(c);
  }

 private:
  std::string text;
  std::string url;
};

#endif
