#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include "EventHandler.h"

#include <string>
#include <EventQueue.h>
#include <FWPlatform.h>

class PlatformThread;
class Event;
class FWPlatform;

class Runnable : public EventHandler {
 public:
  Runnable() { }
  Runnable(const Runnable & other) = delete;
  Runnable & operator= (const Runnable & other) = delete;
  
  void start(PlatformThread * _thread);
  // void stop();

  PlatformThread & getThread();
  const PlatformThread & getThread() const;
  PlatformThread * getThreadPtr() { return thread; }

  void sendEvent(const Event & ev) {
    event_queue.push(0, ev);
  }
        
 protected:
  FWPlatform & getPlatform();
  const FWPlatform & getPlatform() const;
  
  EventQueue event_queue;
  void postEvent(const Event & event);
  virtual void run() = 0;

 private:
  PlatformThread * thread = 0;
};

#endif
