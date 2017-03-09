#ifndef _COMMANDEVENT_H_
#define _COMMANDEVENT_H_

#include <Event.h>

#include <EventHandler.h>

class CommandEvent : public Event {
public:
  CommandEvent(double _timestamp, int _element_id = 0, int _value = 0)
    : Event(_timestamp), element_id(_element_id), value(_value) { }

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
  
 private:
  int element_id, value;
  int edge = +1;
};

#endif
