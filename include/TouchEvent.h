#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_

#include <EventBase.h>

class TouchEvent : public EventBase {
public:
  enum Type {
    ACTION_DOWN, ACTION_MOVE, ACTION_UP, ACTION_CLICK
  };
 TouchEvent(double _timestamp, Type _type, float _x, float _y, long long _identifier)
   : EventBase(_timestamp), type(_type), x(_x), y(_y), identifier(_identifier), flush(false) { }
 TouchEvent(double _timestamp, Type _type, bool _flush = false)
   : EventBase(_timestamp), type(_type), x(0), y(0), identifier(0), flush(_flush) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<TouchEvent>(*this); }
  void dispatch(Element & element) override;
  
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
