#include <InitEvent.h>
#include <Element.h>

void
InitEvent::dispatch(Element & element) {
  element.onInitEvent(*this);
}
