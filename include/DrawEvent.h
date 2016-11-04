#ifndef _DRAWEVENT_H_
#define _DRAWEVENT_H_

#include <EventBase.h>

class DrawEvent : public EventBase {
public:
  DrawEvent() { }

  void dispatch(Element & element) override;
    
 private:  
};

#endif
