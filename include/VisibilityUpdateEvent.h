#ifndef _VISIBILITYUPDATEEVENT_H_
#define _VISIBILITYUPDATEEVENT_H_

#include <Event.h>
#include <EventHandler.h>

#include <string>

class VisibilityUpdateEvent : public Event {
public:
  VisibilityUpdateEvent() { }

  Event * dup() const override { return new VisibilityUpdateEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onVisibilityUpdateEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }
};

#endif
