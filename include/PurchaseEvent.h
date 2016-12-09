#ifndef _PURCHASEEVENT_H_
#define _PURCHASEEVENT_H_

#include <EventBase.h>

class PurchaseEvent : public EventBase {
public:
  enum Type {
    PURCHASE_STATUS
  };
 PurchaseEvent(double _timestamp, Type _type) : EventBase(_timestamp), type(_type) { }

  EventBase * dup() const override { return new PurchaseEvent(*this); }
  void dispatch(Element & element) override;
  
  Type getType() { return type; }

 private:
  Type type;
};

#endif
