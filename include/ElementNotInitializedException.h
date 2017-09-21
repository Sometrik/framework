#ifndef _ELEMENTNOTINITIALIZEDEXCEPTION_H_
#define _ELEMENTNOTINITIALIZEDEXCEPTION_H_

#include <exception>

class ElementNotInitializedException : public std::exception {
 public:
  ElementNotInitializedException() { }

  const char * what() const throw() {
    return "Element not initialized";
  }  
};

#endif
