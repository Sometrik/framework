#include "Runnable.h"

#include <PlatformThread.h>
#include <Event.h>
#include <SysEvent.h>

#include <cassert>
#include <iostream>

using namespace std;

void
Runnable::start(PlatformThread * _thread) {
  assert(_thread);
  thread = _thread;
  try {
    run();
  } catch (exception & e) {
    cerr << "thread died after exception\n";

    SysEvent ev(0.0, SysEvent::THREAD_TERMINATED);
    ev.setThread(thread);
    postEvent(ev);
    throw e;
  }
  SysEvent ev(getPlatform().getTime(), SysEvent::THREAD_TERMINATED);
  ev.setThread(thread);
  postEvent(ev);
}

void
Runnable::postEvent(const Event & event) {
  assert(thread);
  getThread().sendEventFromThread(event);
}

PlatformThread &
Runnable::getThread() {
  assert(thread);
  return *thread;
}
  
const PlatformThread &
Runnable::getThread() const {
  assert(thread);
  return *thread;
}

void
Runnable::logMessage(const char * message) {
  getThread().logMessage(message);
}
  
FWPlatform &
Runnable::getPlatform() {
  return getThread().getPlatform();
}

const FWPlatform &
Runnable::getPlatform() const {
  return getThread().getPlatform();
}

// void
// Runnable::stop() {
//   // assert(thread);
// }

