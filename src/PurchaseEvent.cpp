#include <PurchaseEvent.h>
#include <EventHandler.h>

void
PurchaseEvent::dispatch(EventHandler & element) {
  element.onPurchaseEvent(*this);
  Event::dispatch(element);
}
