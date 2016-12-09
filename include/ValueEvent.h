#ifndef _VALUEEVENT_H_
#define _VALUEEVENT_H_

#include <Event.h>
#include <string>

class ValueEvent : public Event {
public:
  ValueEvent(double _timestamp, const std::string & _text_value)
    : Event(_timestamp), text(_text_value), value(0) { }
  ValueEvent(double _timestamp, int _value)
    : Event(_timestamp), value(_value) { }

  Event * dup() const override { return new ValueEvent(*this); }
  void dispatch(Element & element) override;

  const std::string & getTextValue() const { return text; }
  int getValue() const { return value; }

 private:
  std::string text;
  int value;
};

#endif
