#include <TouchEvent.h>
#include <EventHandler.h>

void
TouchEvent::dispatch(EventHandler & evh) {
  if (!isHandled()) {
    evh.onTouchEvent(*this);
    if (isHandled() && !handler) {
      handler = &evh;
    }
  }
  Event::dispatch(evh);
}
