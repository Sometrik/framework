#ifndef _FORMVIEW_H_
#define _FORMVIEW_H_

#include <FWViewBase.h>
#include <Message.h>

class FormView : public FWViewBase {
 public:
  FormView(int _id) : FWViewBase(_id) { }

  void initialize(FWPlatform * _platform) override {
    FWViewBase::initialize(_platform);
    sendMessage(Message(Message::CREATE_FORMVIEW, getInternalId(), getParentInternalId()));
  }
};

#endif
