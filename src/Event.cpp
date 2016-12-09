#include <Event.h>
#include <EventHandler.h>

void
Event::dispatch(EventHandler & evh) {
  if (!isHandled()) {
    evh.onEvent(*this);
    if (isHandled() && !handler) {
      handler = &evh;
    }
  }
}
