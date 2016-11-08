#include <UpdateEvent.h>
#include <Element.h>

void
UpdateEvent::dispatch(Element & element) {
  element.onUpdateEvent(*this);
  EventBase::dispatch(element);
}
