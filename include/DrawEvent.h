#ifndef _DRAWEVENT_H_
#define _DRAWEVENT_H_

#include <Event.h>

class DrawEvent : public Event {
public:
  DrawEvent(double _timestamp) : Event(_timestamp) { }

  Event * dup() const override { return new DrawEvent(*this); }
  void dispatch(EventHandler & element) override;
  bool isBroadcast() const override { return true; }
    
 private:  
};

#endif
