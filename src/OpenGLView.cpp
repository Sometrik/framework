#include <OpenGLView.h>
#include <FWPlatformBase.h>
#include <Message.h>

void
OpenGLView::initialize(FWPlatformBase * _platform) {
  FWViewBase::initialize(_platform);
  sendMessage(Message(Message::CREATE_OPENGL_VIEW, getId()));
  logical_width = int(_platform->getDisplayWidth() / _platform->getDisplayScale());
  logical_height = int(_platform->getDisplayHeight() / _platform->getDisplayScale());
  actual_width = _platform->getDisplayWidth();
  actual_height = _platform->getDisplayHeight();    
}
