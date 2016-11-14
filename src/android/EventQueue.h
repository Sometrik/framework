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

  void push(EventBase & ev) { data.push_front(ev.dup()); }
  std::shared_ptr<EventBase> pop() {
    pthread_mutex_lock (&_mutex);
    if (!data.empty()) {
      auto ev = data.back();
      data.pop_back();
      pthread_mutex_unlock(&_mutex);
      return ev;
    } else {
      pthread_mutex_unlock(&_mutex);
      return std::shared_ptr<EventBase>(0);
    }
  }
  bool empty() const { return data.empty(); }
  
 private:
  pthread_mutex_t _mutex;
  std::list<std::shared_ptr<EventBase> > data;
};

#endif
