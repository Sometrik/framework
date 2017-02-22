#ifndef _EVENT_H_
#define _EVENT_H_

#include <EventHandler.h>

class Event {
 public:
  Event(double _timestamp) : timestamp(_timestamp) { }
  virtual ~Event() { }

  virtual const char * key() const { return "event"; }
  virtual Event * dup() const = 0;
  virtual void dispatch(EventHandler & evh) {
    if (!isHandled()) {
      evh.onEvent(*this);
      if (isHandled() && !handler) {
	handler = &evh;
      }
    }  
  }
  virtual bool isBroadcast() const { return false; }
  
  double getTimestamp() const { return timestamp; }
  
  bool isHandled() const { return is_handled; }
  bool isRedrawNeeded() const { return redraw_needed; }
  bool isStopped() const { return is_stopped; }

  bool needUpdateLayout() const { return update_layout_needed; }

  void setHandled(bool t = true) { is_handled = t; }
  void requestRedraw() { redraw_needed = true; }
  void updateLayout() { update_layout_needed = true; }
  void stop() { is_stopped = true; }

  bool needUpdate() const { return redraw_needed; }
  void update() { redraw_needed = true; }

  const EventHandler * getHandler() const { return handler; }
  EventHandler * getHandler() { return handler; }

  void setEventHandler(EventHandler * h) { handler = h; }

 protected:
  EventHandler * handler = 0;

 private:
  double timestamp;
  bool is_handled = false, is_stopped = false;
  bool redraw_needed = false;
  bool update_layout_needed = false;
};

#endif
