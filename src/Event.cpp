#include <Event.h>
#include <EventHandler.h>

void
Event::dispatch(EventHandler & evh) {
  if (!isHandled()) {
    element.onEvent(*this);
    if (isHandled() && !handler) {
      handler = &evh;
    }
  }
}
