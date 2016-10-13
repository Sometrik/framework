#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>

class Logger {
 public:
  Logger() { }
  virtual ~Logger() { }
  
  virtual void println(const char * s) = 0;
  void println(const std::string & s) { println(s.c_str()); }  
};

class BasicLogger : Logger{
 public:
  BasicLogger() : Logger() { }
  
  void println(const char * s) override {
    std::cerr << s << std::endl;
  }
};

#endif
