#include <ResizeEvent.h>
#include <EventHandler.h>

void
ResizeEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onResizeEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
