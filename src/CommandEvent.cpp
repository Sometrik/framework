#include <CommandEvent.h>
#include <EventHandler.h>

void
CommandEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onCommandEvent(*this);
  }
  Event::dispatch(element);
}
