#include <FWPlatform.h>

#include <PlatformThread.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <StringUtils.h>

#include <cassert>
#include <iostream>
#include <sys/time.h>
#include <typeinfo>
#include <sstream>

using namespace std;

FWPlatform::FWPlatform(float _display_scale) : display_scale(_display_scale) {
  StringUtils::initialize();
  initialize(this);
}

bool
FWPlatform::run(std::shared_ptr<Runnable> runnable) {
  auto thread = createThread(runnable);
  {
    MutexLocker m(mutex);
    threads[thread->getId()] = thread;
  }
  if (thread->start()) {
    return true;
  } else {
    MutexLocker m(mutex);
    threads.erase(thread->getId());
    return false;
  }
}

void
FWPlatform::terminateThreads() {
  cerr << "terminating " << threads.size() << " threads:\n";
  dumpThreads();

  MutexLocker m(mutex);
  for (auto & thread : threads) {
    thread.second->terminate();
  }
}

void
FWPlatform::onSysEvent(SysEvent & ev) {
  bool exit_app = false;
  if (ev.getType() == SysEvent::THREAD_TERMINATED) {
    MutexLocker m(mutex);
    bool r = false;
    auto it = threads.find(ev.getThread()->getId());
    if (it != threads.end()) {
      threads.erase(it);
      r = true;
    }
    // cerr << "thread " << ev.getThread()->getId() << ":" << ev.getThread() << " is terminated: " << r << "\n";
    if (exit_when_threads_terminated && threads.empty()) {
      exit_app = true;
    }
    assert(r);
  }
  if (exit_app) {
    cerr << "threads exited, terminating\n";
    exitApp();
  }
}

double
FWPlatform::getTime() const {
  struct timeval tv;
  struct timezone tz;
  int r = gettimeofday(&tv, &tz);
  double t = 0;
  if (r == 0) {
    t = (double)tv.tv_sec + tv.tv_usec / 1000000.0;
  }
  return t;
}

void
FWPlatform::postEvent(int internal_id, Event & ev) {
  assert(internal_id);
  auto e = getRegisteredElement(internal_id);
  if (e) {
    ev.dispatch(*e);
  } else {
    std::ostringstream s;
    s << "Failed to dispatch event " << typeid(ev).name() << " id: " << internal_id;
    getLogger().println(s.str());
  }
}

void
FWPlatform::dumpThreads() const {
  MutexLocker m(mutex);
  cerr << "running threads:\n";
  for (auto & t : threads) {
    auto & r = t.second->getRunnable();
    cerr << "  thread " << t.second->getId() << ": " << typeid(r).name() << endl;
  }
}
