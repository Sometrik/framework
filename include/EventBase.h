#ifndef _EVENTBASE_H_
#define _EVENTBASE_H_

class Element;

class EventBase {
 public:
  EventBase(int _originating_id) : originating_id(_originating_id) { }
  virtual ~EventBase() { }

  int getOriginatingId() { return originating_id; }
  virtual void dispatch(Element & element) = 0;
  
 private:
  int originating_id; // internal element id
};

#endif
