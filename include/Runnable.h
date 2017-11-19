#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include <Element.h>

#include <Mutex.h>

#include <exception>
#include <string>

class Runnable : public Element {
 public:
  Runnable() { }
  
  void start(PlatformThread * _thread) {
    initialize(_thread);
    try {
      run();
    } catch (std::exception & e) {
      // std::cerr << "Runnable threw an exception: " + std::string(e.what()) + "\n";
    }
  }

  std::string getStatusText() const {
    MutexLocker m(mutex);
    return status_text;
  }

 protected: 
  virtual void run() = 0;

  void setStatusText(const std::string & s) {
    MutexLocker m(mutex);
    status_text = s;
  }

 private:
  std::string status_text;
  mutable Mutex mutex;
};

#endif
