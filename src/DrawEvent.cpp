#include <DrawEvent.h>
#include <Element.h>

void
DrawEvent::dispatch(Element & element) {
  element.onDrawEvent(*this);
  Event::dispatch(element);
}
