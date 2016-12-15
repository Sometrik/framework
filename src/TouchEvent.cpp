#include <TouchEvent.h>
#include <EventHandler.h>

void
TouchEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onTouchEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
