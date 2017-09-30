#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <Logger.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <Mutex.h>

#include <exception>
#include <memory>
#include <iostream>

class Event;
class FWPlatform;
class HTTPClientFactory;
class Logger;

namespace canvas {
  class ContextFactory;
};

class PlatformThread {
 public:
 PlatformThread(int _id, PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : id(_id), platform(_platform), runnable(_runnable)
    {
    if (_parent_thread) {
      setActualDisplayWidth(_parent_thread->getActualDisplayWidth());
      setActualDisplayHeight(_parent_thread->getActualDisplayHeight());
      setDisplayScale(_parent_thread->getDisplayScale());
    }
  }
  PlatformThread(const PlatformThread & other) = delete;
  PlatformThread & operator=(const PlatformThread & other) = delete;
  
  virtual ~PlatformThread() { }  
  
  virtual bool start() = 0;
  virtual bool testDestroy() = 0;
  virtual void terminate() = 0;
  virtual int sendCommand(const Command & command) = 0;
  virtual void sleep(double t) = 0;
  virtual std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
#ifndef NO_CANVAS
  virtual std::unique_ptr<canvas::ContextFactory> createContextFactory() const = 0;
#endif
  
  Runnable & getRunnable() { return *runnable; }
  Runnable * getRunnablePtr() { return runnable.get(); }
  const Runnable * getRunnablePtr() const { return runnable.get(); }

  FWPlatform & getPlatform() { return *platform; }
  const FWPlatform & getPlatform() const { return *platform; }
  
  int getInternalId() const { return id; }

  void postEvent(int internal_id, const Event & ev) {
    getPlatform().pushEvent(internal_id, ev);
  }

  bool run(std::shared_ptr<Runnable> runnable) {
    auto thread = createThread(runnable);
    {
      MutexLocker m(mutex);
      subthreads[thread->getInternalId()] = thread;
    }
    if (thread->start()) {
      return true;
    } else {
      MutexLocker m(mutex);
      subthreads.erase(thread->getInternalId());
      return false;
    }
  }
  
  virtual std::unique_ptr<Logger> createLogger(const std::string & name) const {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }

  void exitApp() {
    Command c(Command::QUIT_APP, getInternalId());
    sendCommand(c);
  }

  void setActualDisplayWidth(int w) { actual_display_width = w; }
  void setActualDisplayHeight(int h) { actual_display_height = h; }
  void setDisplayScale(float f) { display_scale = f; }

  int getActualDisplayWidth() const { return actual_display_width; }
  int getActualDisplayHeight() const { return actual_display_height; }
  float getDisplayScale() const { return display_scale; }

  void terminateThreads() {
    std::cerr << "terminating " << subthreads.size() << " threads:\n";
    dumpThreads();

    MutexLocker m(mutex);
    for (auto & thread : subthreads) {
      thread.second->terminate();
    }
  }
  void dumpThreads() const {
    MutexLocker m(mutex);
    std::cerr << "running threads:\n";
    for (auto & t : subthreads) {
      auto & r = t.second->getRunnable();
      std::cerr << "  thread " << t.second->getInternalId() << "/" << typeid(r).name() << ": " << r.getStatusText() << std::endl;
    }
  }

  const PlatformThread * getThreadById(int id) const {
    MutexLocker m(mutex);
    auto it = subthreads.find(id);
    if (it != subthreads.end()) {
      return it->second.get();
    } else {
      return 0;
    }
  }

  size_t getNumRunningThreads() const {
    MutexLocker m(mutex);
    return subthreads.size();
  }

  virtual std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) = 0;

 protected:
  virtual void initialize() { }  
  virtual void deinitialize() { }

  void start2() {
    initialize();
    try {
      getRunnable().start(this);
    } catch (std::exception & e) {
      auto logger = createLogger("PlatformThread");
      logger->println("PlatformThread: Runnable threw an exception: " + std::string(e.what()));
    }
    deinitialize();
    
    SysEvent ev(SysEvent::THREAD_TERMINATED);
    ev.setThreadId(getInternalId());
    ev.setRunnable(runnable.get());
    postEvent(0, ev);
  }

  bool exit_when_threads_terminated = false;

 private:
  int id;
  FWPlatform * platform;
  int actual_display_width = 0, actual_display_height = 0;
  float display_scale = 1.0f;
  std::unordered_map<int, std::shared_ptr<PlatformThread> > subthreads;
  std::shared_ptr<Runnable> runnable;

  mutable Mutex mutex;
};

#endif
