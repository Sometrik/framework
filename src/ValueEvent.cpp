#include <ValueEvent.h>
#include <EventHandler.h>

void
ValueEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onValueEvent(*this);
  }
  Event::dispatch(element);
}
