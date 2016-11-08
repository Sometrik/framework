#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_

#include <EventBase.h>

class TouchEvent : public EventBase {
public:
  enum Type {
    ACTION_DOWN, ACTION_MOVE, ACTION_UP
  };
 TouchEvent(Type _type, int _x, int _y, double _timestamp, int _identifier)
   : EventBase(_timestamp), type(_type), x(_x), y(_y), identifier(_identifier) { }

  void dispatch(Element & element) override;
  
  Type getType() const { return type; }
  int getX() const { return x; }
  int getY() const { return y; }
  long getIdentifier() const { return identifier; }

 private:
  Type type;
  int x;
  int y;
  double time;
  int identifier;
};

#endif
