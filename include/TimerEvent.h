#ifndef _TIMEREVENT_H_
#define _TIMEREVENT_H_

#include <Event.h>
#include <EventHandler.h>

class TimerEvent : public Event {
public:
  TimerEvent(int _id) : id(_id) { }

  Event * dup() const override { return new TimerEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onTimerEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }

 private:
  int id;
};

#endif
