#include <ValueEvent.h>
#include <Element.h>

void
ValueEvent::dispatch(Element & element) {
  if (!isHandled()) {
    element.onValueEvent(*this);
  }
  Event::dispatch(element);
}
