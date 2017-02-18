#ifndef _RESIZEEVENT_H_
#define _RESIZEEVENT_H_

#include <Event.h>

class ResizeEvent : public Event {
public:
 ResizeEvent(double _timestamp, int _logical_width, int _logical_height, int _actual_width, int _actual_height )
   : Event(_timestamp), logical_width(_logical_width), logical_height(_logical_height), actual_width(_actual_width), actual_height(_actual_height) { }

  Event * dup() const override { return new ResizeEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onResizeEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }
  bool isBroadcast() const override { return true; }

  int getLogicalWidth() const { return logical_width; }
  int getLogicalHeight() const { return logical_height; }
  int getActualWidth() const { return actual_width; }
  int getActualHeight() const { return actual_height; }
   
 private:
  int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;
};

#endif
