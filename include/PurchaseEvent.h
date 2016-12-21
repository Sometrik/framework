#ifndef _PURCHASEEVENT_H_
#define _PURCHASEEVENT_H_

#include <Event.h>

class PurchaseEvent : public Event {
public:
  enum Type {
    PURCHASE_STATUS
  };
 PurchaseEvent(double _timestamp, Type _type) : Event(_timestamp), type(_type) { }

  Event * dup() const override { return new PurchaseEvent(*this); }
  void dispatch(EventHandler & element) override;
  bool isBroadcast() const override { return true; }
  
  Type getType() { return type; }

 private:
  Type type;
};

#endif
