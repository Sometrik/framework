#include <DrawEvent.h>
#include <EventHandler.h>

void
DrawEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onDrawEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
