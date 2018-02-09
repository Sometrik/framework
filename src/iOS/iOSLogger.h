#ifndef _IOSLOGGER_H_
#define _IOSLOGGER_H_

#include <Logger.h>

#include <cstdio>

class iOSLogger : public Logger {
public:
  iOSLogger(const std::string & _name) : Logger(_name) { }
    
  void println(const char * s) override {
#if 0
    NSLog(@"%s", s);
#else
    fputs(s, stderr);
    fputs("\n", stderr);
    fflush(stderr);
#endif
  }
};

#endif
