#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <memory>
#include <atomic>

class Runnable;
class Event;
class FWPlatform;
class HTTPClientFactory;
class Logger;

namespace canvas {
  class ContextFactory;
};

class PlatformThread {
 public:
 PlatformThread(FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : id(getNextThreadId()),
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
  virtual std::unique_ptr<canvas::ContextFactory> createContextFactory() const = 0;
  
  Runnable & getRunnable() { return *runnable; }
  Runnable * getRunnablePtr() { return runnable.get(); }
  const Runnable * getRunnablePtr() const { return runnable.get(); }

  FWPlatform & getPlatform() { return *platform; }
  const FWPlatform & getPlatform() const { return *platform; }
  
  int getId() const { return id; }

  void sendEventFromThread(const Event & ev);

  bool run(std::shared_ptr<Runnable> runnable);
  std::unique_ptr<Logger> createLogger(const std::string & name) const;

 protected:
  virtual void initialize() { }  
  virtual void deinitialize() { }

  void start2();

  int getNextThreadId() { return next_thread_id.fetch_add(1); }

 private:
  int id;
  FWPlatform * platform;
  std::shared_ptr<Runnable> runnable;

  static std::atomic<int> next_thread_id;
};

#endif
