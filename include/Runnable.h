#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include <Element.h>
#include <Mutex.h>
#include <DateTime.h>

#include <string>

class Logger;

class Runnable : public Element {
 public:
  Runnable() { }
  ~Runnable() {
    unregisterElement(this);
  }
  
  void start(std::shared_ptr<PlatformThread> _thread);
  
  void setStatusText(const std::string & s) {    
    MutexLocker m(mutex);
    status_text = s;       
    heartbeat_time = DateTime::now() / 1000;
  }

  std::pair<std::string, time_t> getStatus() const {
    MutexLocker m(mutex);
    return make_pair(status_text, heartbeat_time);
  }

  virtual std::string getName() const;

  Logger & getLogger();

  bool isVisible() const override {
    return true;
  }

 protected: 
  virtual void run() = 0;

  bool testDestroy();

 private:
  std::string status_text;
  time_t heartbeat_time = 0;
  std::shared_ptr<Logger> logger;
  mutable Mutex mutex;
};

#endif
