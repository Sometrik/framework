#include <DrawEvent.h>
#include <Element.h>

void
DrawEvent::dispatch(Element & element) {
  element.onDrawEvent(*this);
  EventBase::dispatch(element);
}
