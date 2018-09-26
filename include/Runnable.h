#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include <Element.h>
#include <Mutex.h>

#include <string>

class Logger;

class Runnable : public Element {
 public:
  Runnable() { }
  ~Runnable() {
    unregisterElement(this);
  }
  
  void start(std::shared_ptr<PlatformThread> _thread);
  
  std::string getStatusText() const {
    MutexLocker m(mutex);
    return status_text;
  }

  virtual std::string getName() const;

  Logger & getLogger();

  bool isVisible() const override {
    return true;
  }

 protected: 
  virtual void run() = 0;

  void setStatusText(const std::string & s) {
    MutexLocker m(mutex);
    status_text = s;
  }

  bool testDestroy();

 private:
  std::string status_text;
  std::shared_ptr<Logger> logger;
  mutable Mutex mutex;
};

#endif
