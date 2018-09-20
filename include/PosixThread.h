#ifndef _POSIXTHREAD_H_
#define _POSIXTHREAD_H_

#include <PlatformThread.h>
#include <EventQueue.h>

#include <pthread.h>

class PosixThread : public PlatformThread {
public:
 PosixThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(_application, _runnable),
    terminate_thread(false)
  {
    
  }

  bool startThread(std::shared_ptr<PlatformThread> thread) override;
  
  bool testDestroy() override {
    return terminate_thread;
  }

  void sleep(double t) override;
  
  void sendEvent(int internal_id, const Event & ev) override {
    event_queue.push(internal_id, ev);
  }

  void startEventLoop() override {
    auto & runnable = getRunnable();
    while (getNumRunningThreads() != 0 || !testDestroy()) {
      auto ed = event_queue.pop();
      if (ed.first == getInternalId()) {
	ed.second->dispatch(*this);
      } else {
	ed.second->dispatch(runnable);
      }
    }
  }
  
  std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents(bool blocking = true) override {
    std::vector<std::pair<int, std::shared_ptr<Event> > > r;
    if (blocking || !event_queue.empty()) {
      r.push_back(event_queue.pop());
      while (!event_queue.empty()) {
	r.push_back(event_queue.pop());
      }
    }
    return r;
  }

private:
  void setDestroyed() override {
    terminate_thread = true;
  }

  static void * entryPoint(void * pthis);
  
  pthread_t pthread;
  bool terminate_thread = false;
  EventQueue event_queue;
};

#endif
