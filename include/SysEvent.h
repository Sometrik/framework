#ifndef _SYSEVENT_H_
#define _SYSEVENT_H_

#include <Event.h>

class Runnable;

class SysEvent : public Event {
public:
  enum Type {
    START = 1, // Called when app is becoming visible
    RESUME, // Called when the app will start interacting with the user
    PAUSE, // Called when the app is no longer interacting with the user
    STOP, // Called when app is no longer visible
    DESTROY, // Called just before the app is destroyed
    TERMINATE_THREAD,
    THREAD_TERMINATED,
    BACK,
    SHOW_DEBUG
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

  void setThreadId(int id) { thread_id = id; }
  void setRunnable(Runnable * _runnable) { runnable = _runnable; }
  
  Type getType() { return type; }
  int getThreadId() const { return thread_id; }
  const Runnable * getRunnable() const { return runnable; }
  
 private:
  Type type;
  int thread_id = 0;
  Runnable * runnable = 0;
};

#endif
