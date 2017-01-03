#ifndef _PURCHASEEVENT_H_
#define _PURCHASEEVENT_H_

#include <Event.h>

class PurchaseEvent : public Event {
public:
  enum Type {
    PURCHASE_STATUS
  };
 PurchaseEvent(double _timestamp, const char * _productId, Type _type, bool _newPurchase) : Event(_timestamp), type(_type), newPurchase(_newPurchase), productId(_productId) { }

  Event * dup() const override { return new PurchaseEvent(*this); }
  void dispatch(EventHandler & element) override;
  bool isBroadcast() const override { return true; }
  
  Type getType() { return type; }
  bool isNew() { return newPurchase; }
  const char * getProductId() { return productId; }

 private:
  Type type;
  bool newPurchase;
  const char * productId;
};

#endif
