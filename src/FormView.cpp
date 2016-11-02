#include <FormView.h>
#include <Message.h>

void
FormView::initialize(FWPlatformBase * _platform) {
  FWViewBase::initialize(_platform);
  sendMessage(Message(Message::CREATE_FORM_VIEW, getId()));  
}
