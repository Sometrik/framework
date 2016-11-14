#ifndef _UPDATEEVENT_H_
#define _UPDATEEVENT_H_

#include <EventBase.h>

class UpdateEvent : public EventBase {
public:
  UpdateEvent(double _timestamp) : EventBase(_timestamp) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<UpdateEvent>(*this); }
  void dispatch(Element & element) override;
};

#endif
