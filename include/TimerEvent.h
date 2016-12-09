#ifndef _TIMEREVENT_H_
#define _TIMEREVENT_H_

#include <EventBase.h>

class TimerEvent : public EventBase {
public:
  TimerEvent(double _timestamp, int _id) : EventBase(_timestamp), id(_id) { }

  EventBase * dup() const override { return new TimerEvent(*this); }
  void dispatch(Element & element) override;

 private:
  int id;
};

#endif
