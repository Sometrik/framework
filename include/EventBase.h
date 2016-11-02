#ifndef _EVENTBASE_H_
#define _EVENTBASE_H_

class EventBase {
 public:
  EventBase(int _originating_id) : originating_id(_originating_id) { }
  virtual ~EventBase() { }

  int getOriginatingId() { return originating_id; }

 private:
  int originating_id;
};

#endif
