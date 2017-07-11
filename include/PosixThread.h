#ifndef _POSIXTHREAD_H_
#define _POSIXTHREAD_H_

#include <PlatformThread.h>

#include <pthread.h>
#include <atomic>
#include <unistd.h>

class PosixThread : public PlatformThread {
public:
  PosixThread(FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
    : PlatformThread(_platform, _runnable),
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
      
  void sleep(float t) override {
    usleep(int(t * 1000000));
  }
  
protected:
  void sendEventFromThread(const Event & ev) override;
  
private:
  static void * entryPoint(void * pthis);
  
  pthread_t thread;
  pthread_t thread_id;
  std::atomic<bool> terminate_thread;
};

#endif
