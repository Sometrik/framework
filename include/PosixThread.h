#ifndef _POSIXTHREAD_H_
#define _POSIXTHREAD_H_

#include <PlatformThread.h>
#include <EventQueue.h>

#include <unistd.h>
#include <pthread.h>
#include <atomic>

class PosixThread : public PlatformThread {
public:
 PosixThread(PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(_parent_thread, _platform, _runnable),
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

  void recvEvents(EventHandler & evh) override {
    event_queue.recvOne(evh);
    while (!event_queue.empty() && !testDestroy()) {
      event_queue.recvOne(evh);
    }
  }
  
private:
  static void * entryPoint(void * pthis);
  
  pthread_t thread;
  std::atomic<bool> terminate_thread;
  EventQueue event_queue;
};

#endif
