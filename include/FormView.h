#ifndef _FORMVIEW_H_
#define _FORMVIEW_H_

#include <FWViewBase.h>
#include <Command.h>

class FormView : public FWViewBase {
 public:
  FormView(int _id = 0) : FWViewBase(_id) { }

  void initialize(FWPlatform * _platform) override {
    FWViewBase::initialize(_platform);
    sendCommand(Command(Command::CREATE_FORMVIEW, getParentInternalId(), getInternalId()));
  }
};

#endif
