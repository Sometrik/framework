#ifndef _EVENTQUEUE_H_
#define _EVENTQUEUE_H_

#include <EventBase.h>

#include <pthread.h>
#include <list>

class EventQueue {
 public:
  EventQueue() {
    pthread_mutex_init(&_mutex, NULL);
  }
  ~EventQueue() {
    pthread_mutex_destroy(&_mutex);
  }

  void push(int internal_id, EventBase & ev) { data.push_front(std::pair<int, std::shared_ptr<EventBase> >(internal_id, ev.dup())); }
  std::pair<int internal_id, std::shared_ptr<EventBase> > pop() {
    pthread_mutex_lock (&_mutex);
    if (!data.empty()) {
      auto ev = data.back();
      data.pop_back();
      pthread_mutex_unlock(&_mutex);
      return ev;
    } else {
      pthread_mutex_unlock(&_mutex);
      return std::pair<int, std::shared_ptr<EventBase> >(0, std::shared_ptr<EventBase>(0));
    }
  }
  bool empty() const { return data.empty(); }
  
 private:
  pthread_mutex_t _mutex;
  std::list<std::shared_ptr<int, std::shared_ptr<EventBase> > > data;
};

#endif
