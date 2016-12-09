#include <Event.h>
#include <Element.h>

void
Event::dispatch(Element & element) {
  if (!isHandled()) {
    element.onEvent(*this);
  }
}
