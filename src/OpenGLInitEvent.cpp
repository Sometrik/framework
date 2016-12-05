#include <OpenGLInitEvent.h>
#include <Element.h>

void
OpenGLInitEvent::dispatch(Element & element) {
  if (!isHandled()) {
    element.onOpenGLInitEvent(*this);
  }
  EventBase::dispatch(element);
}
