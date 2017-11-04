#ifndef _SCROLLCHANGEDEVENT_H_
#define _SCROLLCHANGEDEVENT_H_

#include <Event.h>

class ScrollChangedEvent : public Event {
public:
 ResizeEvent(int _scroll_pos, int _scroll_rem)
   : scroll_pos(_scroll_pos), scroll_rem(_scroll_rem) { }

  Event * dup() const override { return new ScrollChangedEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onScrollChangedEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }

  int getScrollPos() const { return scroll_pos; }
  int getScrollRem() const { return scroll_rem; }
   
 private:
  int scroll_pos, scroll_rem;
};

#endif
