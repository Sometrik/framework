#include <OpenGLInitEvent.h>
#include <EventHandler.h>

void
OpenGLInitEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onOpenGLInitEvent(*this);
  }
  // Do not call super class.
}
