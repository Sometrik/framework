#ifndef _INITEVENT_H_
#define _INITEVENT_H_

#include <EventBase.h>

class InitEvent : public EventBase {
public:
  InitEvent(double _timestamp) : EventBase(_timestamp) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<InitEvent>(*this); }
  void dispatch(Element & element) override;
    
 private:  
};

#endif
