#include <PurchaseEvent.h>
#include <Element.h>

void
PurchaseEvent::dispatch(Element & element) {
  element.onPurchaseEvent(*this);
  Event::dispatch(element);
}
