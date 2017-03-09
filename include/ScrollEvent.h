#ifndef _SCROLLEVENT_H_
#define _SCROLLEVENT_H_

#include <Event.h>

#include <EventHandler.h>

class ScrollEvent : public Event {
 public:
 ScrollEvent(double _timestamp, float _delta)
   : Event(_timestamp), delta(_delta) { }

  const char * key() const override { return "scroll"; }
  Event * dup() const override { return new ScrollEvent(*this); }
  void dispatch(EventHandler & evh) override {
    if (!isHandled()) {
      evh.onScrollEvent(*this);
      if (isHandled() && !handler) {
	handler = &evh;
      }
    }
    Event::dispatch(evh);
  }
  bool isBroadcast() const override { return true; }

  float getDelta() const { return delta; }
  
 private:
  float delta;
};

#endif
