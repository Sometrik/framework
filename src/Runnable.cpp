#include "Runnable.h"

#include <PlatformThread.h>
#include <Event.h>

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

void
Runnable::terminate() {
  if (thread) thread->terminate();
}

bool
Runnable::testDestroy() const {
  return thread ? thread->testDestroy() : true;
}
