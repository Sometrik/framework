#include <FWPlatform.h>

#include <PlatformThread.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <StringUtils.h>
#include <PosixThread.h>

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

void
FWPlatform::run(std::shared_ptr<Runnable> runnable) {
  auto thread = run2(runnable);
  if (thread.get()) threads.push_back(thread);
}

std::shared_ptr<PlatformThread>
FWPlatform::run2(std::shared_ptr<Runnable> & runnable) {
  auto thread = make_shared<PosixThread>(this, runnable);
  if (thread->start()) {
    return thread;
  } else {
    return shared_ptr<PlatformThread>(0);
  }
}

void
FWPlatform::terminateThreads() {
  cerr << "terminating " << threads.size() << " threads\n";
  for (auto & thread : threads) {
    assert(thread.get());
    thread->terminate();
  }
}

void
FWPlatform::onSysEvent(SysEvent & ev) {
  if (ev.getType() == SysEvent::THREAD_TERMINATED) {
    cerr << "thread " << ev.getThread() << " is terminated\n";
    bool is_affected = false;
    for (auto it = threads.begin(); it != threads.end(); it++) {
      PlatformThread * thread = it->get();
      if (thread == ev.getThread()) {
	threads.erase(it);
	is_affected = true;
	break;
      }
    }
    if (exit_when_threads_terminated && threads.empty()) {
      cerr << "threads exited, terminating\n";
      exit();
    }
    // assert(is_affected);
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
  cerr << "threads:\n";
  for (auto & t : threads) {
    auto & r = t->getRunnable();
    cerr << "  " << typeid(r).name() << endl;
  }
}
