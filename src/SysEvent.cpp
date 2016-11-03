#include <SysEvent.h>
#include <Element.h>

void
SysEvent::dispatch(Element & element) {
  element.onSysEvent(*this);
}
