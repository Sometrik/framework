#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <Event.h>

#include <string>

class Runnable;

class SysEvent : public Event {
public:
  enum Type {
    START = 1, // Called when app is becoming visible
    RESUME, // Called when the app will start interacting with the user
    PAUSE, // Called when the app is no longer interacting with the user
    STOP, // Called when app is no longer visible
    DESTROY, // Called just before the app is destroyed
    MEMORY_WARNING,
    TERMINATE_THREAD,
    THREAD_TERMINATED,
    LANGUAGE_CHANGED,
    BACK,
    DEBUG
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
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }

  void setTextValue(const std::string & _text_value) { text_value = _text_value; }
  void setThreadId(int id) { thread_id = id; }
  void setRunnable(Runnable * _runnable) { runnable = _runnable; }
  
  Type getType() { return type; }
  const std::string & getTextValue() const { return text_value; }
  int getThreadId() const { return thread_id; }
  const Runnable * getRunnable() const { return runnable; }
  
 private:
  Type type;
  std::string text_value;
  int thread_id = 0;
  Runnable * runnable = 0;
};

#endif
