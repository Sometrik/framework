#ifndef _PURCHASEEVENT_H_
#define _PURCHASEEVENT_H_

#include <Event.h>

#include <EventHandler.h>

#include <string>

class PurchaseEvent : public Event {
public:
  enum Type {
    PURCHASE_STATUS
  };
 PurchaseEvent(double _timestamp, const char * _productId, Type _type, bool _newPurchase) : Event(_timestamp), type(_type), newPurchase(_newPurchase), productId(_productId) { }

  Event * dup() const override { return new PurchaseEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onPurchaseEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }
  
  Type getType() const { return type; }
  bool isNew() const { return newPurchase; }
  const std::string & getProductId() const { return productId; }

 private:
  Type type;
  bool newPurchase;
  std::string productId;
};

#endif
