#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include <Element.h>
#include <Command.h>

class TextLabel : public Element {
 public:
 TextLabel(const std::string & _value, unsigned int _flags = 0, bool _autolink = false) : Element(0, _flags), value(_value), autolink(_autolink) { }
 TextLabel(const std::string & _value, const std::string & _url) : Element(0, 1), value(_value), url(_url), autolink(false) { }

  bool isA(const std::string & className) const override {
    if (className == "TextLabel") return true;
    return Element::isA(className);
  }

  void text(const std::string & _value) override {
    value = _value;
    
    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(value);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_TEXT, getParentInternalId(), getInternalId());
    c.setTextValue(value);
    c.setTextValue2(url);
    c.setValue(autolink ? 1 : 0);
    c.setFlags(getFlags());
    sendCommand(c);
  }

 private:
  std::string value;
  std::string url;
  bool autolink;
};

#endif
