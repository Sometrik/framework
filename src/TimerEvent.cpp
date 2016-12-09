#include <TimerEvent.h>
#include <EventHandler.h>

void
TimerEvent::dispatch(EventHandler & element) {
  element.onTimerEvent(*this);
  Event::dispatch(element);
}
