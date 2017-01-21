#include <FWPlatform.h>

#include <pthread.h>
#include <PlatformThread.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <StringUtils.h>

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <typeinfo>
#include <sstream>

using namespace std;

class PosixThread : public PlatformThread {
public:
  PosixThread(FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
    : PlatformThread(_platform, _runnable)
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
    // platform = 0;
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
    getPlatform().pushEvent(ev);
  }
  
private:
  static void * entryPoint(void * pthis);
  
  pthread_t thread;
  pthread_t thread_id;
  volatile bool terminate_thread = false;
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

FWPlatform::FWPlatform(float _display_scale) : display_scale(_display_scale) {
  StringUtils::initialize();
  initialize(this);
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
  std::shared_ptr<PlatformThread> thread(new PosixThread(this, runnable));
  thread->start();
  return thread;
}

void
FWPlatform::terminateThreads() {
  cerr << "terminating " << threads.size() << " threads\n";
  for (auto & thread : threads) {
    assert(thread.get());
    thread->terminate();
  }
}

void
FWPlatform::disconnectThreads() {
  for (auto & thread : threads) {
    thread->disconnect();
  }
}

void
FWPlatform::onSysEvent(SysEvent & ev) {
  if (ev.getType() == SysEvent::THREAD_TERMINATED) {
    for (auto it = threads.begin(); it != threads.end(); it++) {
      PlatformThread * thread = it->get();
      if (thread == ev.getThread()) {
	threads.erase(it);
	num_running_threads--;
	break;
      }
    }
  }
}

double
FWPlatform::getTime() const {
  struct timeval tv;
  struct timezone tz;
  int r = gettimeofday(&tv, &tz);
  double t = 0;
  if (r == 0) {
    t = (double)tv.tv_sec + tv.tv_usec / 1000000.0;
  }
  return t;
}

void
FWPlatform::postEvent(int internal_id, Event & ev) {
  assert(internal_id);
  Element * e = getElementByInternalId(internal_id);
  if (e) ev.dispatch(*e);
  else {
    std::ostringstream s;
    s << "Failed to dispatch event " << typeid(ev).name() << " id: " << internal_id;
    getLogger().println(s.str());
  }
}
