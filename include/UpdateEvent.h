#ifndef _UPDATEEVENT_H_
#define _UPDATEEVENT_H_

#include <EventBase.h>

class UpdateEvent : public EventBase {
public:
  UpdateEvent(double _timestamp) : EventBase(_timestamp) { }

  EventBase * dup() const override { return new UpdateEvent(*this); }
  void dispatch(Element & element) override;
};

#endif
