#ifndef _UPDATEEVENT_H_
#define _UPDATEEVENT_H_

#include <Event.h>

class UpdateEvent : public Event {
public:
  UpdateEvent(double _timestamp) : Event(_timestamp) { }

  Event * dup() const override { return new UpdateEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onUpdateEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }
};

#endif
