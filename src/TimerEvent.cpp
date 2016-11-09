#include <TimerEvent.h>
#include <Element.h>

void
TimerEvent::dispatch(Element & element) {
  element.onTimerEvent(*this);
  EventBase::dispatch(element);
}
