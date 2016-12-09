#include <TouchEvent.h>
#include <Element.h>

void
TouchEvent::dispatch(Element & element) {
  element.onTouchEvent(*this);
  Event::dispatch(element);
}
