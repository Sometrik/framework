class TouchEvent {
public:
  enum Type{
      ACTION_DOWN, ACTION_MOVE, ACTION_UP
    };
  TouchEvent(Type _type, int _x, int _y, double _timestamp, int _identifier) : type(_type), x(_x), y(_y), time(_timestamp), id(_identifier){

  }
  ~TouchEvent(){

  }

  Type getType(){ return type; }
  int getX(){ return x; }
  int getY(){ return y; }
  double timestamp(){ return time; }
  long identifier(){ return id; }

 private:
  Type type;
  int x;
  int y;
  double time;
  int id;
};
