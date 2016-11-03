#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <EventBase.h>

class SysEvent : public EventBase {
public:
  enum Type {
    SHUTDOWN,
    MEMORY_WARNING,
    THREAD_TERMINATED
  };
  SysEvent(int _originating_id, Type _type)
    : EventBase(_originating_id), type(_type) { }

  void dispatch(Element & element) override;
  
  Type getType() { return type; }

 private:
  Type type;
};

#endif
