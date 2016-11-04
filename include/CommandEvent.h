#ifndef _COMMANDEVENT_H_
#define _COMMANDEVENT_H_

#include <EventBase.h>

class CommandEvent : public EventBase {
public:
  CommandEvent(int _value = 0) : value(_value) { }

  void dispatch(Element & element) override;
    
 private:
  int value;
};

#endif
