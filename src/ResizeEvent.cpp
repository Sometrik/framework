#include <ResizeEvent.h>
#include <Element.h>

void
ResizeEvent::dispatch(Element & element) {
  element.onResizeEvent(*this);
  Event::dispatch(element);
}
