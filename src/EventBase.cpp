#include <EventBase.h>

void
EventBase::dispatch(Element & element) {
  if (!isHandled()) {
    element.onEvent(*this);
  }
}
