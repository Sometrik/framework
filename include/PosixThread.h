#ifndef _POSIXTHREAD_H_
#define _POSIXTHREAD_H_

#include <PlatformThread.h>
#include <EventQueue.h>

#include <unistd.h>
#include <pthread.h>
#include <atomic>

class PosixThread : public PlatformThread {
public:
 PosixThread(PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(_parent_thread, _platform, _application, _runnable),
    terminate_thread(false)
  {
    
  }

  bool start() override;
  
  bool testDestroy() override {
    return terminate_thread;
  }

  void terminate() override {
    terminate_thread = true;    
  }

  // void postEventToThread(Event & event) {
  //   event.dispatch(getRunnable());
  // }

  void sleep(double t) override {
    usleep((unsigned int)(t * 1000000));
  }

  void sendEvent(int internal_id, const Event & ev) override {
    event_queue.push(internal_id, ev);
  }

  void startEventLoop() override {
    auto & runnable = getRunnable();
    while (!testDestroy()) {
      event_queue.recvOne(runnable);
      while (!event_queue.empty() && !testDestroy()) {
        event_queue.recvOne(runnable);
      }
    }
  }
  
  std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents() override {
    std::vector<std::pair<int, std::shared_ptr<Event> > > r;
    r.push_back(event_queue.pop());
    while (!event_queue.empty()) {
      r.push_back(event_queue.pop());
    }
    return r;
  }

private:
  static void * entryPoint(void * pthis);
  
  pthread_t thread;
  std::atomic<bool> terminate_thread;
  EventQueue event_queue;
};

#endif
