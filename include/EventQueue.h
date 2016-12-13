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
    pthread_cond_init(&_condv, NULL);
  }
  ~EventQueue() {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_condv);
  }

  void push(int internal_id, const Event & ev) {
    std::shared_ptr<Event> ptr(ev.dup());
    pthread_mutex_lock (&_mutex);
    data.push_front(std::pair<int, std::shared_ptr<Event> >(internal_id, ptr));
    pthread_cond_signal(&_condv);
    pthread_mutex_unlock(&_mutex);
  }
  std::pair<int, std::shared_ptr<Event> > pop() {
    pthread_mutex_lock (&_mutex);
    while (data.empty()) {
      pthread_cond_wait(&_condv, &_mutex);
    }
    auto ev = data.back();
    data.pop_back();
    pthread_mutex_unlock(&_mutex);
    return ev;
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

  void recvOne(EventHandler & evh) {
    auto ed = pop();
    ed.second->dispatch(evh);
  }
  
 private:
  mutable pthread_mutex_t _mutex;
  pthread_cond_t _condv;
  std::list<std::pair<int, std::shared_ptr<Event> > > data;
};

#endif
