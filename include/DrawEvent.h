#ifndef _DRAWEVENT_H_
#define _DRAWEVENT_H_

#include <EventBase.h>

class DrawEvent : public EventBase {
public:
  DrawEvent(double _timestamp) : EventBase(_timestamp) { }

  EventBase * dup() const override { return new DrawEvent(*this); }
  void dispatch(Element & element) override;
    
 private:  
};

#endif
