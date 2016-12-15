#include <OpenGLInitEvent.h>
#include <EventHandler.h>

void
OpenGLInitEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onOpenGLInitEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  // Do not call super class.
}
