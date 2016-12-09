#include <UpdateEvent.h>
#include <EventHandler.h>

void
UpdateEvent::dispatch(EventHandler & element) {
  element.onUpdateEvent(*this);
  Event::dispatch(element);
}
