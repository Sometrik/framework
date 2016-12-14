#include <PurchaseEvent.h>
#include <EventHandler.h>

void
PurchaseEvent::dispatch(EventHandler & element) {
  if (!isHandled()) {
    element.onPurchaseEvent(*this);
    if (isHandled() && !handler) {
      handler = &element;
    }
  }
  Event::dispatch(element);
}
