#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_

#include <EventBase.h>

class TouchEvent : public EventBase {
public:
  enum Type {
    ACTION_DOWN, ACTION_MOVE, ACTION_UP
  };
 TouchEvent(Type _type, int _x, int _y, double _timestamp, int _identifier)
   : type(_type), x(_x), y(_y), time(_timestamp), identifier(_identifier) { }

  void dispatch(Element & element) override;
  
  Type getType() { return type; }
  int getX() { return x; }
  int getY() { return y; }
  double getTimestamp() { return time; }
  long getIdentifier() { return identifier; }

 private:
  Type type;
  int x;
  int y;
  double time;
  int identifier;
};

#endif
