#include <ResizeEvent.h>
#include <EventHandler.h>

void
ResizeEvent::dispatch(EventHandler & element) {
  element.onResizeEvent(*this);
  Event::dispatch(element);
}
