#ifndef _TIMEREVENT_H_
#define _TIMEREVENT_H_

#include <EventBase.h>

class TimerEvent : public EventBase {
public:
  TimerEvent(double _timestamp) : EventBase(_timestamp) { }

  void dispatch(Element & element) override;
    
 private:  
};

#endif
