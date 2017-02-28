#ifndef _ANDROID_COMPATABILITY_H_
#define _ANDROID_COMPATABILITY_H_

#include <string>
#include <sstream>

  template <typename T>
  std::string to_string(T value) {
      std::ostringstream os ;
      os << value ;
      return os.str() ;
  }

  float
  stof(std::string value){
    const char * cstring = value.c_str();
    float f;
    sscanf(cstring, "%f", &f);
    return f;
  }

  int
  stoi(std::string value){
    std::stringstream buffer(value);
    int i;
    buffer >> i;
    return i;
  }

  double
  stod(std::string value){
    std::stringstream buffer(value);
    double d;
    buffer >> d;
    return d;
  }




#endif
