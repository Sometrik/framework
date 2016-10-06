#ifndef _ANDROIDLOGGER_H_
#define _ANDROIDLOGGER_H_

#include <Logger.h>
#include <android/log.h>

class AndroidLogger : public Logger {
 public:
  AndroidLogger() { }

  void println(const char * s) override {
    __android_log_print(ANDROID_LOG_ERROR, "Sometrik", "resize: %d %d ", width, height);
  }
};

#endif
