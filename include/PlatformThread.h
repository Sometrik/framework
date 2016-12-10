#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <memory>

class Runnable;
class Event;

class PlatformThread {
 public:
  PlatformThread(std::shared_ptr<Runnable> & _runnable)
    : runnable(_runnable) {
    
  }
  virtual ~PlatformThread() { }  
  
  virtual void start() = 0;
  virtual bool testDestroy() = 0;
  virtual void sendEventFromThread(const Event & ev) = 0;
  virtual void terminate() = 0;
  virtual void disconnect() = 0;
  virtual void logMessage(const char * message) = 0;
  virtual void sleep(float t) = 0;
  virtual void alert(const char * message) { }
  
  Runnable & getRunnable() { return *runnable; }
  Runnable * getRunnablePtr() { return runnable.get(); }
  const Runnable * getRunnablePtr() const { return runnable.get(); }

 protected:
    
 private:
  std::shared_ptr<Runnable> runnable;
};

#endif
