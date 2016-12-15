#include <TimerEvent.h>
#include <EventHandler.h>

void
TimerEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onTimerEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
