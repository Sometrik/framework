#include "Thread.h"

#include <iostream>
#include <signal.h>

using namespace std;

Thread::Thread(const string & _thread_name) : thread_name(_thread_name)
{
}

Thread::~Thread() {
  
}

// wait_thread();

bool
Thread::start() {
  is_running = true;
  started_time = time(0);
  if (pthread_create(&thread, NULL, Thread::entryPoint, this) == 0) {
    return true;
  } else {
    return false;
  }
}

bool
Thread::detach() {
  if (pthread_detach(thread) == 0) {
    return true;
  } else {
    onCreateThreadFailure();
    return false;
  }
}

bool
Thread::join() {
  if (pthread_join(thread, NULL) == 0) {
    return true;
  } else {
    return false;
  }
}

void *
Thread::entryPoint(void * pthis) {
  signal(SIGPIPE, SIG_IGN);

  Thread * pt = static_cast<Thread*>(pthis);
  pt->thread_id = pthread_self();

  pt->setupThread();
  pt->run();
  pt->deinitializeThread();

  {
    MutexLocker l(pt->mutex);
    pt->is_running = false;
  }

  // cerr << "thread really exiting\n";
  pthread_exit(0);
  
  return 0;
}

bool
Thread::isRunning() {
  bool t;
  {
    MutexLocker l(mutex);
    t = is_running;
  }
  return t;
}

#if 0
float
Thread::sleep(float sec) const {
  int t = int(sec * 1000);
  while (t > 0 && !exit_thread) {
    int t2 = t > 250 ? 250 : t;

#ifdef WIN32
    Sleep(t2);
#else
    struct timespec req = { t2 / 1000, (t2 % 1000) * 1000000 };
    nanosleep(&req, NULL);
#endif
    
    t -= t2;
  }
  if (t) {
    return t / 1000.0f;
  } else {
    return 0;
  }
}
#endif
