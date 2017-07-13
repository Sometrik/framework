#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include "EventHandler.h"

#include <EventQueue.h>
#include <FWPlatform.h>
#include <PlatformThread.h>

#include <string>

class PlatformThread;
class Event;
class FWPlatform;

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

  void terminate() { getThread().terminate(); }
  bool testDestroy() { return getThread().testDestroy(); }

  PlatformThread & getThread() { return *thread; }
  const PlatformThread & getThread() const { return *thread; }
  PlatformThread * getThreadPtr() { return thread; }

 protected:
  FWPlatform & getPlatform() { return getThread().getPlatform(); }
  const FWPlatform & getPlatform() const { return getThread().getPlatform(); }
  
  // EventQueue event_queue;
  void postEvent(const Event & event) {
    getThread().sendEventFromThread(event);
  }
  virtual void run() = 0;

 private:
  PlatformThread * thread = 0;
};

#endif
