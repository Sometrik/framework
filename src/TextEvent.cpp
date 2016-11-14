#include <TextEvent.h>
#include <Element.h>

void
TextEvent::dispatch(Element & element) {
  if (!isHandled()) {
    element.onTextEvent(*this);
  }
  EventBase::dispatch(element);
}
