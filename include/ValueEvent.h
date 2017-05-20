#ifndef _VALUEEVENT_H_
#define _VALUEEVENT_H_

#include <Event.h>

#include <EventHandler.h>

#include <string>

class ValueEvent : public Event {
public:
  ValueEvent(double _timestamp, const std::string & _text_value)
    : Event(_timestamp), text(_text_value), value(0), value2(0) { }
  ValueEvent(double _timestamp, int _value, int _value2 = 0)
    : Event(_timestamp), value(_value), value2(_value2) { }

  Event * dup() const override { return new ValueEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onValueEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }

  const std::string & getTextValue() const { return text; }
  int getValue() const { return value; }
  int getValue2() const { return value2; }

 private:
  std::string text;
  int value, value2;
};

#endif
