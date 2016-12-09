#include <SysEvent.h>
#include <EventHandler.h>

void
SysEvent::dispatch(EventHandler & element) {
  element.onSysEvent(*this);
  // Do not call super class.
}
