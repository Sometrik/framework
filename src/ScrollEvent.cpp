#include <ScrollEvent.h>
#include <EventHandler.h>

void
ScrollEvent::dispatch(EventHandler & evh) {
  if (!isHandled()) {
    evh.onScrollEvent(*this);
    if (isHandled() && !handler) {
      handler = &evh;
    }
  }
  Event::dispatch(evh);
}
