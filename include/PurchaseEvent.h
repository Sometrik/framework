#ifndef _PURCHASEEVENT_H_
#define _PURCHASEEVENT_H_

#include <EventBase.h>

class PurchaseEvent : public EventBase {
public:
  enum Type {
    PURCHASE_STATUS
  };
 PurchaseEvent(double _timestamp, Type _type) : EventBase(_timestamp), type(_type) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<PurchaseEvent>(*this); }
  void dispatch(Element & element) override;
  
  Type getType() { return type; }

 private:
  Type type;
};

#endif
