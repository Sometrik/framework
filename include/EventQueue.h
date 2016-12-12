#ifndef _EVENTQUEUE_H_
#define _EVENTQUEUE_H_

#include <Event.h>

#include <memory>
#include <pthread.h>
#include <list>
#include <vector>

class EventQueue {
 public:
  EventQueue() {
    pthread_mutex_init(&_mutex, NULL);
  }
  ~EventQueue() {
    pthread_mutex_destroy(&_mutex);
  }

  void push(int internal_id, const Event & ev) {
    std::shared_ptr<Event> ptr(ev.dup());
    pthread_mutex_lock (&_mutex);
    data.push_front(std::pair<int, std::shared_ptr<Event> >(internal_id, ptr));
    pthread_mutex_unlock(&_mutex);
  }
  std::pair<int, std::shared_ptr<Event> > pop() {
    pthread_mutex_lock (&_mutex);
    if (!data.empty()) {
      auto ev = data.back();
      data.pop_back();
      pthread_mutex_unlock(&_mutex);
      return ev;
    } else {
      pthread_mutex_unlock(&_mutex);
      return std::pair<int, std::shared_ptr<Event> >(0, std::shared_ptr<Event>(0));
    }
  }
  bool empty() const {
    pthread_mutex_lock(&_mutex);
    bool e = data.empty();
    pthread_mutex_unlock(&_mutex);
    return e;
  }

  std::vector<std::shared_ptr<Event> > recvAll() {
    pthread_mutex_lock(&_mutex);
    std::vector<std::shared_ptr<Event> > v;    
    while (!data.empty()) {
      v.push_back(data.front().second);
      data.pop_front();
    }
    pthread_mutex_unlock(&_mutex);
    return v;
  }

  size_t recv(EventHandler & evh) {
    auto events = recvAll();    
    for (auto & ev : events) {
      ev->dispatch(evh);     
    }
    return events.size();
  }
  
 private:
  mutable pthread_mutex_t _mutex;
  std::list<std::pair<int, std::shared_ptr<Event> > > data;
};

#endif