#ifndef _ANDROIDLOGGER_H_
#define _ANDROIDLOGGER_H_

#include <Logger.h>
#include <android/log.h>

class AndroidLogger : public Logger {
 public:
 AndroidLogger(const std::string & _name) : Logger(_name) { }

  void println(const char * s) override {
    std::string r = std::string(s);
    __android_log_print(ANDROID_LOG_INFO, getName().c_str(), s, 0);
  }
};

#endif
