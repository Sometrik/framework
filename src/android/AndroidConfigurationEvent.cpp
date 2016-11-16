#include <AndroidConfigurationEvent.h>
#include <Element.h>

void
AndroidConfigurationEvent::dispatch(Element & element) {
  if (!isHandled()) {
    element.onConfigurationEvent(*this);
  }
  EventBase::dispatch(element);
}
