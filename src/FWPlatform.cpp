#include <FWPlatform.h>

#include <pthread.h>
#include <PlatformThread.h>
#include <Runnable.h>

#include <iostream>
#include <unistd.h>

using namespace std;

class PosixThread : public PlatformThread {
public:
  PosixThread(std::shared_ptr<Runnable> & _runnable, FWPlatform * _platform)
    : PlatformThread(_runnable),
      platform(_platform)
  {
    
  }

  void start() override {
    if (pthread_create(&thread, NULL, PosixThread::entryPoint, this) == 0) {
      
    } else {
      cerr << "failed to create thread\n";
    }
  }
  
  bool testDestroy() override {
    return false; // !terminate_thread;
  }

  void terminate() override {
    assert(0);
    terminate_thread = true;    
  }

  void disconnect() override {
    platform = 0;
  }

  void logMessage(const char * message) override {
    // wxLogMessage(wxString::FromUTF8(message));
  }

  // void postEventToThread(Event & event) {
  //   event.dispatch(getRunnable());
  // }
      
  void sleep(float t) override {
    usleep(int(t * 1000000));
  }
  
protected:
  void sendEventFromThread(const Event & ev) override {
    if (platform) {
      platform->pushEvent(ev);
    }
  }
  
private:
  static void * entryPoint(void * pthis);
  
  pthread_t thread;
  pthread_t thread_id;
  volatile bool terminate_thread = false;
  FWPlatform * platform;
};

void *
PosixThread::entryPoint(void * pthis) {
  // OS::getInstance().ignoreSigPipe();

  PosixThread * pt = static_cast<PosixThread*>(pthis);
  pt->thread_id = pthread_self();

  // pt->setupThread();
  pt->getRunnable().start(pt);
  // pt->getRunnable().stop();
  // pt->deinitializeThread();

  // {
  //   MutexLocker l(pt->mutex);
  //   pt->is_running = false;
  // }

  // cerr << "thread really exiting\n";
  pthread_exit(0);
  
  return 0;
}

std::shared_ptr<PlatformThread>
FWPlatform::run(std::shared_ptr<Runnable> runnable) {
  auto thread = run2(runnable);
  num_running_threads++;
  threads.push_back(thread);
  return thread;
}

std::shared_ptr<PlatformThread>
FWPlatform::run2(std::shared_ptr<Runnable> & runnable) {
  std::shared_ptr<PlatformThread> thread(new PosixThread(runnable, this));
  thread->start();
  return thread;
}
