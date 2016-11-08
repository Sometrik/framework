#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_

#include <EventBase.h>

class TouchEvent : public EventBase {
public:
  enum Type {
    ACTION_DOWN, ACTION_MOVE, ACTION_UP
  };
 TouchEvent(Type _type, float _x, float _y, double _timestamp, int _identifier)
   : EventBase(_timestamp), type(_type), x(_x), y(_y), identifier(_identifier) { }

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
