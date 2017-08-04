#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <Logger.h>
#include <Runnable.h>
#include <SysEvent.h>

#include <exception>
#include <memory>

class Event;
class FWPlatform;
class HTTPClientFactory;
class Logger;

namespace canvas {
  class ContextFactory;
};

class PlatformThread {
 public:
 PlatformThread(int _id, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : id(_id),
    platform(_platform),
    runnable(_runnable)
    {
    
  }
  PlatformThread(const PlatformThread & other) = delete;
  PlatformThread & operator=(const PlatformThread & other) = delete;
  
  virtual ~PlatformThread() { }  
  
  virtual bool start() = 0;
  virtual bool testDestroy() = 0;
  virtual void terminate() = 0;
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

  void postEvent(const Event & ev) {
    getPlatform().pushEvent(ev);
  }

  bool run(std::shared_ptr<Runnable> runnable) {
    return getPlatform().run(runnable);
  }
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const {
    return getPlatform().createLogger(name);
  }

 protected:
  virtual void initialize() { }  
  virtual void deinitialize() { }

  void start2() {
    initialize();
    try {
      getRunnable().start(this);
    } catch (std::exception & e) {
      getPlatform().getLogger().println("PlatformThread: Runnable threw an exception: " + std::string(e.what()));
    }
    deinitialize();
    
    SysEvent ev(SysEvent::THREAD_TERMINATED);
    ev.setThreadId(getInternalId());
    postEvent(ev);
  }

 private:
  int id;
  FWPlatform * platform;
  std::shared_ptr<Runnable> runnable;
};

#endif
