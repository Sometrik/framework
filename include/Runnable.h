#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include "EventHandler.h"

#include <EventQueue.h>
#include <FWPlatform.h>
#include <Mutex.h>

#include <string>

class Event;
class FWPlatform;
class PlatformThread;

class Runnable : public EventHandler {
 public:
  Runnable() { }
  Runnable(const Runnable & other) = delete;
  Runnable & operator= (const Runnable & other) = delete;
  
  void start(PlatformThread * _thread) {
    if (_thread) {
      thread = _thread;
      run();
    }
  }

#if 0
  void sendEvent(const Event & ev) {
    event_queue.push(0, ev);
  }
#endif

  PlatformThread & getThread() { return *thread; }
  const PlatformThread & getThread() const { return *thread; }
  PlatformThread * getThreadPtr() { return thread; }

  std::string getStatusText() const {
    MutexLocker m(mutex);
    return status_text;
  }

 protected: 
  virtual void run() = 0;

  void setStatusText(const std::string & s) {
    MutexLocker m(mutex);
    status_text = s;
  }

  // EventQueue event_queue;

 private:
  PlatformThread * thread = 0;
  std::string status_text;
  mutable Mutex mutex;
};

#endif
