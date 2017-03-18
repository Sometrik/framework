#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <memory>

class Runnable;
class Event;
class FWPlatform;

class PlatformThread {
 public:
 PlatformThread(FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : platform(_platform), runnable(_runnable) {
    
  }
  virtual ~PlatformThread() { }  
  
  virtual void start() = 0;
  virtual bool testDestroy() = 0;
  virtual void sendEventFromThread(const Event & ev) = 0;
  virtual void terminate() = 0;
  virtual void disconnect() = 0;
  virtual void sleep(float t) = 0;
  virtual void alert(const char * message) { }
  
  Runnable & getRunnable() { return *runnable; }
  Runnable * getRunnablePtr() { return runnable.get(); }
  const Runnable * getRunnablePtr() const { return runnable.get(); }

  FWPlatform & getPlatform() { return *platform; }
  const FWPlatform & getPlatform() const { return *platform; }
  
 protected:
    
 private:
  FWPlatform * platform;
  std::shared_ptr<Runnable> runnable;
};

#endif
