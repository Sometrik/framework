#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <Event.h>

#include <string>

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
    END_MODAL,
    LANGUAGE_CHANGED
  };
 SysEvent(Type _type) : type(_type) { }

  Event * dup() const override { return new SysEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onSysEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    // Do not call super class.
  }
  bool isBroadcast() const override { return true; }

  void setValue(int _value) { value = _value; }
  void setTextValue(const std::string & _text_value) { text_value = _text_value; }
  void setThreadId(int id) { thread_id = id; }
  
  Type getType() { return type; }
  int getValue() { return value; }
  const std::string & getTextValue() const { return text_value; }
  int getThreadId() const { return thread_id; }
  
 private:
  Type type;
  int value = 0;
  std::string text_value;
  int thread_id;
};

#endif
