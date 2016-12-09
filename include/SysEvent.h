#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <EventBase.h>

#include <string>

class SysEvent : public EventBase {
public:
  enum Type {
    START = 1, // Called when app is becoming visible
    RESUME, // Called when the app will start interacting with the user
    PAUSE, // Called when the app is no longer interacting with the user
    STOP, // Called when app is no longer visible
    DESTROY, // Called just before the app is destroyed
    MEMORY_WARNING,
    THREAD_TERMINATED,
    END_MODAL
  };
 SysEvent(double _timestamp, Type _type) : EventBase(_timestamp), type(_type) { }

  EventBase * dup() const override { return new SysEvent(*this); }
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
