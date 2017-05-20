#ifndef _COMMANDEVENT_H_
#define _COMMANDEVENT_H_

#include <Event.h>

#include <EventHandler.h>

class CommandEvent : public Event {
public:
 CommandEvent(double _timestamp, int _element_id = 0, int _value = 0, int _value2 = 0)
   : Event(_timestamp), element_id(_element_id), value(_value), value2(_value2) { }

  Event * dup() const override { return new CommandEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onCommandEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  
  int getElementId() const { return element_id; }
  int getValue() const { return value; }
  int getValue2() const { return value2; }
  
 private:
  int element_id, value, value2;
  int edge = +1;
};

#endif
