#include <CommandEvent.h>
#include <Element.h>

void
CommandEvent::dispatch(Element & element) {
  element.onCommandEvent(*this);
}
