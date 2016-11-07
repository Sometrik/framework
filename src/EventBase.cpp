#include <EventBase.h>
#include <Element.h>

void
EventBase::dispatch(Element & element) {
  if (!isHandled()) {
    element.onEvent(*this);
  }
}
