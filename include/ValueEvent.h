#ifndef _VALUEEVENT_H_
#define _VALUEEVENT_H_

#include <EventBase.h>
#include <string>

class ValueEvent : public EventBase {
public:
  ValueEvent(double _timestamp, const std::string & _text_value)
    : EventBase(_timestamp), text_value(_text_value), value(0) { }
  ValueEvent(double _timestamp, int _value)
    : EventBase(_timestamp), value(_value) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<ValueEvent>(*this); }
  void dispatch(Element & element) override;

  const std::string & getTextValue() const { return text_value; }
  int getValue() const { return value; }

 private:
  std::string text;
  int value;
};

#endif
