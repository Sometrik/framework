#include <UpdateEvent.h>
#include <EventHandler.h>

void
UpdateEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onUpdateEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
