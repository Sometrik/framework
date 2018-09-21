#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>

class Logger {
 public:
  Logger(const std::string & _name) : name(_name) { }
  virtual ~Logger() { }
  
  virtual void println(const char * s) = 0;
  void println(const std::string & s) { println(s.c_str()); }

  const std::string & getName() const { return name; }
  
 private:
  std::string name;
};

class DummyLogger : public Logger {
 public:
  DummyLogger() : Logger("") { }

  void println(const char * s) override { }
};

#endif
