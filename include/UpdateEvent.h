#ifndef _UPDATEEVENT_H_
#define _UPDATEEVENT_H_

#include <EventBase.h>

class UpdateEvent : public EventBase {
public:
  UpdateEvent(double _timestamp) : EventBase(_timestamp) { }

  void dispatch(Element & element) override;
    
 private:  
};

#endif
