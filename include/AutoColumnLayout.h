#ifndef _AUTOCOLUMNLAYOUT_H_
#define _AUTOCOLUMNLAYOUT_H_

#include <Element.h>
#include <Command.h>

class AutoColumnLayout : public Element {
 public:
  AutoColumnLayout() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_AUTO_COLUMN_LAYOUT, getParentInternalId(), getInternalId()));
    initializeChildren();
  }
};

#endif
