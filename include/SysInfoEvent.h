#ifndef _SYSINFOEVENT_H_
#define _SYSINFOEVENT_H_

#include <Event.h>

#include <string>

class Runnable;

class SysInfoEvent : public Event {
public:
  enum Type {
    START = 1, // Called when app is becoming visible
    RESUME, // Called when the app will start interacting with the user
    PAUSE, // Called when the app is no longer interacting with the user
    STOP, // Called when app is no longer visible
    DESTROY, // Called just before the app is destroyed
    MEMORY_WARNING,
    LANGUAGE_CHANGED
  };
 SysInfoEvent(Type _type) : type(_type) { }

  Event * dup() const override { return new SysInfoEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onSysInfoEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }

  bool isBroadcast() const override { return true; }

  void setTextValue(const std::string & _text_value) { text_value = _text_value; }
  
  Type getType() { return type; }
  const std::string & getTextValue() const { return text_value; }
  
 private:
  Type type;
  std::string text_value;
};

#endif