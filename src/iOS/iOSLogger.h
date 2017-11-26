#ifndef _IOSLOGGER_H_
#define _IOSLOGGER_H_

#include <Logger.h>

#include <iostream>

class iOSLogger : public Logger {
public:
  iOSLogger(const std::string & _name) : Logger(_name) { }
    
  void println(const char * s) override {
    NSLog(@"%s", s);
  }
};

#endif
