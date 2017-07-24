#ifndef _VISIBILITYEVENT_H_
#define _VISIBILITYEVENT_H_

#include <Event.h>
#include <EventHandler.h>

#include <string>

class VisibilityEvent : public Event {
public:
  VisibilityEvent(bool _visible) : visible(_visible) { }

  Event * dup() const override { return new VisibilityEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onVisibilityEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }

  bool isVisible() const { return visible; }

 private:
  bool visible;
};

#endif
