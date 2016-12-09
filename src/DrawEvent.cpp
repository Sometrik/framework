#include <DrawEvent.h>
#include <EventHandler.h>

void
DrawEvent::dispatch(EventHandler & element) {
  element.onDrawEvent(*this);
  Event::dispatch(element);
}
