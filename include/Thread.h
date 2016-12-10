#ifndef _THREAD_H_
#define _THREAD_H_

#include "Mutex.h"

#include <pthread.h>
#include <string>

class Thread {
 public:
  Thread(const std::string & _thread_name);
  virtual ~Thread();
  
  bool start();
  bool join();
  bool detach();

  void exit() { exit_thread = true; }
  const std::string & getName() const { return thread_name; }
  
  bool isRunning();
  time_t getStartedTime() const { return started_time; }
  
  pthread_t getThreadId() const { return thread_id; }

 protected:
  
  // float sleep(float sec) const;
  
  virtual void setupThread() { }
  virtual void deinitializeThread() { }
  virtual void run() = 0;
  virtual void onCreateThreadFailure() { }

  bool exit_thread = false;
  
 private:
  static void * entryPoint(void * pthis);

  std::string thread_name;
  pthread_t thread;
  time_t started_time = 0;
  volatile bool is_running = false;
  pthread_t thread_id;
  Mutex mutex;
};

#endif
