#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include <Element.h>
#include <Command.h>

class TextLabel : public Element {
 public:
 TextLabel(const std::string & _value, bool _autolink = false, book _markdown = false) : value(_value), autolink(_autolink), markdown(_markdown) { }

  bool isA(const std::string & className) const override {
    if (className == "TextLabel") return true;
    return Element::isA(className);
  }

  void text(const std::string & _value) override {
    value = _value;
    
    if (isInitialized()) {
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setTextValue(value);
      sendCommand(c);
    }
  }

  void setLink(const std::string & text) {
    Command c(Command::CREATE_LINK, getInternalId());
    c.setTextValue(text);
    sendCommand(c);
  }

  const std::string & getText() const { return value; }

 protected:
  void create() override {
    Command c(Command::CREATE_TEXT, getParentInternalId(), getInternalId());
    c.setTextValue(value);
    c.setValue((autolink ? 1 : 0) | (markdown ? 2 : 0));
    c.setFlags(getFlags());
    sendCommand(c);
  }

 private:
  std::string value;
  bool autolink, markdown;
};

#endif
