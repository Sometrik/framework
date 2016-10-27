class TouchEvent {
public:
  enum Type{
      ACTION_DOWN, ACTION_MOVE, ACTION_UP
    };
  TouchEvent(Type _type, int _x, int _y, double _timestamp, long _identifier) : type(_type), x(_x), y(_y), timestamp(_timestamp), identifier(_identifier){

  }
  ~TouchEvent(){

  }

  Type getType(){ return type; }
  int getX(){ return x; }
  int getY(){ return y; }
  double timestamp(){ return timestamp; }
  long identifier(){ return identifier; }

 private:
  Type type;
  int x;
  int y;
  double timestamp;
  long identifier;
};

#endif
