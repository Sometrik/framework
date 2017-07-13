#include <PlatformThread.h>

#include <FWPlatform.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <Logger.h>

#include <exception>

using namespace std;

atomic<int> PlatformThread::next_thread_id(1);

void
PlatformThread::sendEventFromThread(const Event & ev) {
  getPlatform().pushEvent(ev);
}

void
PlatformThread::start2() {
  initialize();
  try {
    getRunnable().start(this);
  } catch (exception & e) {
    getPlatform().getLogger().println("PlatformThread: Runnable threw an exception: " + string(e.what()));
  }
  deinitialize();

  SysEvent ev(getPlatform().getTime(), SysEvent::THREAD_TERMINATED);
  ev.setThread(this);
  sendEventFromThread(ev);
}
