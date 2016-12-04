#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <EventBase.h>

#include <string>

class SysEvent : public EventBase {
public:
  enum Type {
    SHUTDOWN,
    MEMORY_WARNING,
    THREAD_TERMINATED,
    RESUME,
    PAUSE,
    STOP,
    RESTART,
    END_MODAL
  };
 SysEvent(double _timestamp, Type _type) : EventBase(_timestamp), type(_type) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<SysEvent>(*this); }
  void dispatch(Element & element) override;

  void setValue(int _value) { value = _value; }
  void setTextValue(const std::string & _text_value) { text_value = _text_value; }
  
  Type getType() { return type; }
  int getValue() { return value; }
  const std::string & getTextValue() const { return text_value; }

 private:
  Type type;
  int value = 0;
  std::string text_value;
};

#endif
