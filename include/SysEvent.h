#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <Event.h>

#include <string>

class PlatformThread;

class SysEvent : public Event {
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
 SysEvent(double _timestamp, Type _type) : Event(_timestamp), type(_type) { }

  Event * dup() const override { return new SysEvent(*this); }
  void dispatch(EventHandler & element) override;

  void setValue(int _value) { value = _value; }
  void setTextValue(const std::string & _text_value) { text_value = _text_value; }
  void setThread(PlatformThread * _thread) { thread = _thread; }
  
  Type getType() { return type; }
  int getValue() { return value; }
  const std::string & getTextValue() const { return text_value; }
  const PlatformThread * getThread() const { return thread; }
  
 private:
  Type type;
  int value = 0;
  std::string text_value;
  PlatformThread * thread = 0;
};

#endif
