#ifndef _PURCHASEEVENT_H_
#define _PURCHASEEVENT_H_

#include <EventBase.h>

class PurchaseEvent : public EventBase {
public:
  enum Type {
    PURCHASE_STATUS
  };
  PurchaseEvent(Type _type) : type(_type) { }

  void dispatch(Element & element) override;
  
  Type getType() { return type; }

 private:
  Type type;
};

#endif
