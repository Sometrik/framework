#include "Runnable.h"

#include <PlatformThread.h>
#include <Event.h>
#include <SysEvent.h>

#include <cassert>

using namespace std;

void
Runnable::start(PlatformThread * _thread) {
  assert(_thread);
  thread = _thread;
  try {
    run();
  } catch (exception & e) {
    getPlatform().getLogger().println("Runnable: received an exception: " + string(e.what()));
  }
#if 0
  {
    MutexLocker l(mutex);
    is_running = false;
  }
#endif
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
  
FWPlatform &
Runnable::getPlatform() {
  return getThread().getPlatform();
}

const FWPlatform &
Runnable::getPlatform() const {
  return getThread().getPlatform();
}

#if 0
bool
Runnable::isRunning() const {
  bool t;
  {
    MutexLocker l(mutex);
    t = is_running;
  }
  return t;
}
#endif

void
Runnable::terminate() {
  if (thread) thread->terminate();
}

bool
Runnable::testDestroy() const {
  return thread ? thread->testDestroy() : true;
}
