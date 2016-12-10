#ifndef _MUTEX_H_
#define _MUTEX_H_

#if (defined _WXWIDGETS && defined _WIN32)
#include <wx/thread.h>
#else
#include <pthread.h>
#endif

class Mutex {
 public:
  Mutex();
  ~Mutex();

  Mutex(const Mutex & other) = delete;
  Mutex & operator=(const Mutex & other) = delete;
  
  void lock();
  void unlock();
  
 private:
#if (defined _WXWIDGETS && defined _WIN32)
  wxMutex m;
#else
  pthread_mutex_t m;
#endif
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
