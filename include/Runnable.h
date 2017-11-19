#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include <Element.h>
#include <Mutex.h>
#include <Logger.h>

#include <exception>
#include <string>

class Logger;

class Runnable : public Element {
 public:
  Runnable() { }
  
  void start(PlatformThread * _thread) {
    initialize(_thread);
    try {
      run();
    } catch (std::exception & e) {
      getLogger().println("exception: " + std::string(e.what()));
    }
  }

  std::string getStatusText() const {
    MutexLocker m(mutex);
    return status_text;
  }

  virtual std::string getName() const;

  Logger & getLogger();

 protected: 
  virtual void run() = 0;

  void setStatusText(const std::string & s) {
    MutexLocker m(mutex);
    status_text = s;
  }

 private:
  std::string status_text;
  std::shared_ptr<Logger> logger;
  mutable Mutex mutex;
};

#endif
