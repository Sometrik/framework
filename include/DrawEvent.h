#ifndef _DRAWEVENT_H_
#define _DRAWEVENT_H_

#include <EventBase.h>

class DrawEvent : public EventBase {
public:
  DrawEvent(int _originating_id) : EventBase(_originating_id) { }

  void dispatch(Element & element) override;
    
 private:  
};

#endif
