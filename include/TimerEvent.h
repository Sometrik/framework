#ifndef _TIMEREVENT_H_
#define _TIMEREVENT_H_

#include <Event.h>

class TimerEvent : public Event {
public:
  TimerEvent(double _timestamp, int _id) : Event(_timestamp), id(_id) { }

  Event * dup() const override { return new TimerEvent(*this); }
  void dispatch(Element & element) override;

 private:
  int id;
};

#endif
