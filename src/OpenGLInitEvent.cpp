#include <OpenGLInitEvent.h>
#include <Element.h>

void
OpenGLInitEvent::dispatch(Element & element) {
  if (!isHandled()) {
    element.onOpenGLInitEvent(*this);
  }

  if (!isHandled()){
    for (auto & c: element.getChildren()){
      dispatch(*c);
    }
  }
  //Do not call super class.
}
