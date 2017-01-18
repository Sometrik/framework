#ifndef _SCROLLVIEW_H_
#define _SCROLLVIEW_H_

#include <Element.h>
#include <Command.h>

class ScrollView : public Element {
 public:
  ScrollView() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_SCROLLVIEW, getParentInternalId(), getInternalId()));
    initializeChildren();
  }
};

#endif
