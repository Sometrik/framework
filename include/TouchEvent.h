#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_

#include <Event.h>

class TouchEvent : public Event {
public:
  enum Type {
    ACTION_DOWN, ACTION_DRAG, ACTION_UP
  };
 TouchEvent(double _timestamp, Type _type, float _x, float _y, long long _identifier)
   : Event(_timestamp), type(_type), x(_x), y(_y), identifier(_identifier), flush(false) { }
 TouchEvent(double _timestamp, Type _type, bool _flush = false)
   : Event(_timestamp), type(_type), x(0), y(0), identifier(0), flush(_flush) { }

  Event * dup() const override { return new TouchEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      evh.onTouchEvent(*this);
      if (isHandled() && !handler) {
	handler = &evh;
      }
    }
    Event::dispatch(evh);
  }
  bool isBroadcast() const override { return true; }
  
  Type getType() const { return type; }
  float getX() const { return x; }
  float getY() const { return y; }
  long long getIdentifier() const { return identifier; }
  bool isFlush() const { return flush; }

 private:
  Type type;
  float x, y;
  long long identifier;
  bool flush;
};

#endif
