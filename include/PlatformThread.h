#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <memory>
#include <InternalFormat.h>

class Runnable;
class Event;

namespace canvas {
  class Image;
};

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
  virtual std::shared_ptr<canvas::Image> loadImageFromMemory(const unsigned char * buffer, size_t size, canvas::InternalFormat target_format, unsigned int required_width = 0, unsigned int required_height = 0, unsigned int requested_levels = 1) const = 0;
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
