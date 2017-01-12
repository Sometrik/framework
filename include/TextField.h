#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>

class TextField : public Element {
 public:
 TextField() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_TEXTFIELD, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
  }

  const std::string & getValue() { return value; }
			       
 private:
  std::string value;
};

#endif
