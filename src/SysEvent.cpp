#include <SysEvent.h>
#include <EventHandler.h>

void
SysEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onSysEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  // Do not call super class.
}
