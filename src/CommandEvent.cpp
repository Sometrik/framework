#include <CommandEvent.h>
#include <EventHandler.h>

void
CommandEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onCommandEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
