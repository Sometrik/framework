#include <PosixThread.h>
#include <FWPlatform.h>
#include <Runnable.h>

#include <iostream>

using namespace std;

bool
PosixThread::start() {
  int r = pthread_create(&thread, NULL, PosixThread::entryPoint, this);
  if (r == 0) {
    return true;
  } else {
    cerr << "failed to create thread: " << r << "\n";
    return false;
  }
}

void
PosixThread::sendEventFromThread(const Event & ev) {
  getPlatform().pushEvent(ev);
}

void *
PosixThread::entryPoint(void * pthis) {
  PosixThread * pt = static_cast<PosixThread*>(pthis);
  pt->thread_id = pthread_self();

  pt->getRunnable().start(pt);
  pthread_exit(0);
  
  return 0;
}
