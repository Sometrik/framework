#include "Runnable.h"

#include <PlatformThread.h>
#include <Event.h>
#include <ThreadEvent.h>

#include <cassert>
#include <iostream>

using namespace std;

void
Runnable::start(PlatformThread * _thread) {
  cerr << "starting runnable\n";
  assert(_thread);
  thread = _thread;
  run();
  ThreadEvent ev(0.0, ThreadEvent::THREAD_STOPPED, thread);
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
  
// void
// Runnable::stop() {
//   // assert(thread);
// }

