#ifndef _ACTIONBAR_H_
#define _ACTIONBAR_H_

#include <InputElement.h>

#include <Command.h>
#include <FWPlatform.h>

class ActionBar : public Element {
 public:
  ActionBar() { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_ACTIONBAR, getParentInternalId(), getInternalId());
    sendCommand(c);
  }
};

#endif
