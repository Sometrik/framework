#include <TouchEvent.h>
#include <EventHandler.h>

void
TouchEvent::dispatch(EventHandler & element) {
  element.onTouchEvent(*this);
  Event::dispatch(element);
}
