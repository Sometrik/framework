#ifndef _COMMANDEVENT_H_
#define _COMMANDEVENT_H_

#include <EventBase.h>

class CommandEvent : public EventBase {
public:
  CommandEvent(int _originating_id, int _value = 0) : EventBase(_originating_id), value(_value) { }

 private:
  int value;
};

#endif
