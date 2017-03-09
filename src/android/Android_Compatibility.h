#ifndef _ANDROID_COMPATABILITY_H_
#define _ANDROID_COMPATABILITY_H_

#include <string>
#include <sstream>

namespace std {
  template <typename T> inline
   std::string to_string(T value) {
      std::ostringstream os ;
      os << value ;
      return os.str() ;
  }

  inline float
  stof(std::string value){
    const char * cstring = value.c_str();
    float f;
    sscanf(cstring, "%f", &f);
    return f;
  }

  inline int
  stoi(std::string value){
    std::stringstream buffer(value);
    int i;
    buffer >> i;
    return i;
  }

  inline double
  stod(std::string value){
    std::stringstream buffer(value);
    double d;
    buffer >> d;
    return d;
  }

  inline long
  stoll(std::string value){
    std::stringstream buffer(value);
    long l;
    buffer >> l;
    return l;
  }

  inline unsigned long stoull(std::string value) {
  std::stringstream buffer(value);
  unsigned long l;
  buffer >> l;
  return l;
}
};



#endif
