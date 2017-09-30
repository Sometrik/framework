#include <FWPlatform.h>

#include <SysEvent.h>
#include <StringUtils.h>
#include <FWApplication.h>

#include <cassert>
#include <iostream>
#include <typeinfo>
#include <sstream>

using namespace std;

atomic<int> FWPlatform::next_thread_id(1);

#if 0
void
FWPlatform::onSysEvent(SysEvent & ev) {
  bool exit_app = false;
  if (ev.getType() == SysEvent::THREAD_TERMINATED) {
    MutexLocker m(mutex);
    bool r = false;
    auto it = threads.find(ev.getThreadId());
    if (it != threads.end()) {
      threads.erase(it);
      r = true;
    }
    if (exit_when_threads_terminated && threads.empty()) {
      exit_app = true;
    }
    assert(r);
  }
  if (exit_app) {
    cerr << "threads exited, terminating\n";
    getThread().exitApp();
  }
}
#endif

void
FWPlatform::postEvent(int internal_id, Event & ev) {
  assert(internal_id);
  auto e = getRegisteredElement(internal_id);
  if (e) {
    ev.dispatch(*e);
  } else {
    std::ostringstream s;
    s << "Failed to dispatch event " << typeid(ev).name() << " id: " << internal_id;
    getApplication().getLogger().println(s.str());
  }
}
