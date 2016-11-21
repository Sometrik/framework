#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_

#include <Element.h>
#include <Command.h>
#include <TextEvent.h>

class TextField : public Element {
 public:
 TextField() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_TEXTFIELD, getParentInternalId(), getInternalId()));
  }

  void onTextEvent(TextEvent & ev) override {
    value = ev.getText();
  }

  const std::string & getValue() { return value; }
			       
 private:
  std::string value;
};

#endif
