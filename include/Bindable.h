#ifndef _BINDABLE_H_
#define _BINDABLE_H_

#include <string>

class Bindable {
 public:
  Bindable() { }
  virtual ~Bindable() { }
  
  Bindable(const Bindable & other) = delete;
  Bindable & operator =(const Bindable & other) = delete;

  virtual void call() = 0;
  virtual void call(bool t) = 0;
  virtual void call(int i) = 0;
  virtual void call(const std::string & s) = 0;  
};

#endif
