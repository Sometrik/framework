#include <Event.h>
#include <EventHandler.h>

void
Event::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onEvent(*this);
  }
}
