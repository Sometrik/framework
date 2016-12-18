#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

class Mutex {
 public:
  Mutex();
  ~Mutex();

  Mutex(const Mutex & other) = delete;
  Mutex & operator=(const Mutex & other) = delete;
  
  void lock();
  void unlock();
  
 private:
  pthread_mutex_t m;
};

class MutexLocker {
 public:
  MutexLocker(Mutex & _mutex);
  ~MutexLocker();

  void release();

 private:
  Mutex * mutex;
};

#endif
