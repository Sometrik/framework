#include <PosixThread.h>

#include <iostream>

using namespace std;

bool
PosixThread::start() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  int r = pthread_create(&thread, &attr, PosixThread::entryPoint, this);
  pthread_attr_destroy(&attr);
  if (r == 0) {
    return true;
  } else {
    cerr << "failed to create thread: " << r << "\n";
    return false;
  }
}

void *
PosixThread::entryPoint(void * pthis) {
  PosixThread * pt = static_cast<PosixThread*>(pthis);
  pt->start2();  
  pthread_exit(0);
  
  return 0;
}
