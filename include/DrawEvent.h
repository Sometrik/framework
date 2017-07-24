#ifndef _DRAWEVENT_H_
#define _DRAWEVENT_H_

#include <Event.h>

class DrawEvent : public Event {
public:
  DrawEvent() { }

  Event * dup() const override { return new DrawEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onDrawEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }
};

#endif
