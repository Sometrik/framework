#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_

#include <EventBase.h>

class TouchEvent : public EventBase {
public:
  enum Type {
    ACTION_DOWN, ACTION_MOVE, ACTION_UP, ACTION_CLICK
  };
 TouchEvent(double _timestamp, Type _type, float _x, float _y, int _identifier)
   : EventBase(_timestamp), type(_type), x(_x), y(_y), identifier(_identifier) { }
 TouchEvent(double _timestamp, Type _type)
   : EventBase(_timestamp), type(_type), x(0), y(0), identifier(0) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<TouchEvent>(*this); }
  void dispatch(Element & element) override;
  
  Type getType() const { return type; }
  float getX() const { return x; }
  float getY() const { return y; }
  long getIdentifier() const { return identifier; }

 private:
  Type type;
  float x, y;
  int identifier;
};

#endif
