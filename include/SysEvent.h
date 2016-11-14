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
 SysEvent(double _timestamp, Type _type) : EventBase(_timestamp), type(_type) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<SysEvent>(*this); }
  void dispatch(Element & element) override;
  
  Type getType() { return type; }

 private:
  Type type;
};

#endif
