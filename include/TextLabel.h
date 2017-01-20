#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include <Element.h>
#include <Command.h>

class TextLabel : public Element {
 public:
  TextLabel(const std::string & _text, unsigned int _flags = 0) : Element(0, _flags), text(_text) { }
 TextLabel(const std::string & _text, const std::string & _url) : Element(0, 1), text(_text), url(_url) { }

  bool isA(const std::string & className) override {
    if (className == "TextLabel") return true;
    return Element::isA(className);
  }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_TEXT, getParentInternalId(), getInternalId());
    c.setTextValue(text);
    c.setTextValue2(url);
    c.setFlags(getFlags());
    sendCommand(c);
  }

 private:
  std::string text;
  std::string url;
};

#endif
