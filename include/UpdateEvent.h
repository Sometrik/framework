#ifndef _UPDATEEVENT_H_
#define _UPDATEEVENT_H_

#include <Event.h>

class UpdateEvent : public Event {
public:
  UpdateEvent(double _timestamp) : Event(_timestamp) { }

  Event * dup() const override { return new UpdateEvent(*this); }
  void dispatch(Element & element) override;
};

#endif
