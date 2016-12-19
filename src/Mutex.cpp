#include "Mutex.h"

Mutex::Mutex() {
  pthread_mutex_init(&m, 0);
}

Mutex::~Mutex() {
  pthread_mutex_destroy( &m );  
}

void
Mutex::lock() {
  pthread_mutex_lock( &m );
}

void
Mutex::unlock() {
  pthread_mutex_unlock( &m );
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
