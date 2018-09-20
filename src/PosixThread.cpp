#include <PosixThread.h>

#include <cassert>
#include <unistd.h>

using namespace std;

struct ptr_holder_s {
  std::shared_ptr<PlatformThread> data;
};

bool
PosixThread::startThread(std::shared_ptr<PlatformThread> thread) {
  assert(thread.get());

  ptr_holder_s * holder = new ptr_holder_s;
  holder->data = thread;
  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  
  int r = pthread_create(&pthread, &attr, PosixThread::entryPoint, holder);
  pthread_attr_destroy(&attr);
  if (r == 0) {
    return true;
  } else {
    delete holder;
    return false;
  }
}

void
PosixThread::sleep(double t) {
  usleep((unsigned int)(t * 1000000));
}

void *
PosixThread::entryPoint(void * pthis) {
  auto * holder = static_cast<ptr_holder_s *>(pthis);
  assert(holder);
  if (holder) {
    cerr << "got holder " << holder->data.get() << endl;
    auto & ptr = holder->data;
    assert(ptr.get());
    if (ptr.get()) {
      PosixThread::startThread2(ptr);
    }
  }
  delete holder;
    
  pthread_exit(0);
  
  return 0;
}
