#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

class Mutex {
 public:
  Mutex() {
    pthread_mutex_init(&m, 0);
  }
  ~Mutex() {
    pthread_mutex_destroy( &m );  
  }

  Mutex(const Mutex & other) = delete;
  Mutex & operator=(const Mutex & other) = delete;
  
  void lock() {
    pthread_mutex_lock( &m );
  }
  void unlock() {
    pthread_mutex_unlock( &m );
  }
  
 private:
  pthread_mutex_t m;
};

class MutexLocker {
 public:
  MutexLocker(Mutex & _mutex) : mutex(&_mutex) {
    mutex->lock();
  }
  ~MutexLocker() {
    release();
  }

  void release() {
    if (mutex) {
      mutex->unlock();
      mutex = 0;
    }
  }

 private:
  Mutex * mutex;
};

#endif
