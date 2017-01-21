#ifndef _BINDABLE_H_
#define _BINDABLE_H_

#include <string>

class Bindable {
 public:
  Bindable() = default;
  virtual ~Bindable() = default;
  
  Bindable(const Bindable & other) = delete;
  Bindable & operator =(const Bindable & other) = delete;

  virtual bool call() = 0;
  virtual bool call(bool t) = 0;
  virtual bool call(int i) = 0;
  virtual bool call(const std::string & s) = 0;  
};

#endif
