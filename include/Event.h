#ifndef _EVENT_H_
#define _EVENT_H_

#include <EventHandler.h>

#define FW_EVENT_IS_HANDLED	1
#define FW_EVENT_IS_STOPPED	2
#define FW_EVENT_REDRAW		4
#define FW_EVENT_UPDATE_LAYOUT	8

class Event {
 public:
  Event() { }
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
    
  bool isHandled() const { return flags & FW_EVENT_IS_HANDLED; }
  bool isRedrawNeeded() const { return flags & FW_EVENT_REDRAW; }
  bool isStopped() const { return flags & FW_EVENT_IS_STOPPED; }

  bool needUpdateLayout() const { return flags & FW_EVENT_UPDATE_LAYOUT; }

  void setHandled(bool t = true) { setFlag(FW_EVENT_IS_HANDLED, t); }
  void requestRedraw() { setFlag(FW_EVENT_REDRAW, true); }
  void updateLayout() { setFlag(FW_EVENT_UPDATE_LAYOUT, true); }
  void stop() { setFlag(FW_EVENT_IS_STOPPED, true); }

  bool needUpdate() const { return flags & FW_EVENT_REDRAW; }
  void update() { setFlag(FW_EVENT_REDRAW, true); }

  const EventHandler * getHandler() const { return handler; }
  EventHandler * getHandler() { return handler; }

  void setEventHandler(EventHandler * h) { handler = h; }

 protected:
  void setFlag(unsigned short flag, bool value) {
    if (value) {
      flags |= flag;
    } else {
      flags &= ~flag;
    }
  }
  
  EventHandler * handler = 0;

 private:
  unsigned short flags = 0;
};

#endif
