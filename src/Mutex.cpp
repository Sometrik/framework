#include "Mutex.h"

Mutex::Mutex() {
#if !(defined _WXWIDGETS && defined _WIN32)
  pthread_mutex_init(&m, 0);
#endif
}

Mutex::~Mutex() {
#if !(defined _WXWIDGETS && defined _WIN32)
  pthread_mutex_destroy( &m );  
#endif
}

void
Mutex::lock() {
#if (defined _WXWIDGETS && defined _WIN32)
  m.Lock();
#else
  pthread_mutex_lock( &m );
#endif
}

void
Mutex::unlock() {
#if (defined _WXWIDGETS && defined _WIN32)
  m.Unlock();
#else
  pthread_mutex_unlock( &m );
#endif
}

MutexLocker::MutexLocker(Mutex & _mutex) : mutex(&_mutex) {
  mutex->lock();
}

MutexLocker::~MutexLocker() {
  release();
}

void
MutexLocker::release() {
  if (mutex) {
    mutex->unlock();
    mutex = 0;
  }
}
