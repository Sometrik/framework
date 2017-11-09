#include <PosixThread.h>

#include <cassert>
#include <unistd.h>

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
    return false;
  }
}

void
PosixThread::sleep(double t) {
  usleep((unsigned int)(t * 1000000));
}

void *
PosixThread::entryPoint(void * pthis) {
  PosixThread * pt = static_cast<PosixThread*>(pthis);
  assert(pt);
  pt->start2();  
  pthread_exit(0);
  
  return 0;
}
