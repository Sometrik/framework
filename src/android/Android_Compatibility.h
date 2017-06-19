#ifndef _ANDROID_COMPATABILITY_H_
#define _ANDROID_COMPATABILITY_H_

#include <string>
#include <sstream>
#include <cstdlib>

namespace std {
  template <typename T>
    inline string to_string(T value) {
    ostringstream os ;
    os << value ;
    return os.str() ;
  }

  inline int stoi(const string & str, size_t* idx = 0, int base = 10) {
#if 1
    return strtoul(str.c_str(), 0, base);
#else
    stringstream buffer(str);
    int i;
    buffer >> i;
    return i;
#endif
  }

  inline float stof(string value) {
#if 1
    stringstream buffer(value);
    float f;
    buffer >> f;
    return f;
#else
    const char * cstring = value.c_str();
    float f;
    sscanf(cstring, "%f", &f);
    return f;
#endif
  }


  inline unsigned long long stoull(string value) {
     stringstream buffer(value);
     unsigned long long l;
     buffer >> l;
     return l;
   }

  inline double stod(string value) {
    stringstream buffer(value);
    double d;
    buffer >> d;
    return d;
  }

  inline long long stoll(string value){
    stringstream buffer(value);
    long long l;
    buffer >> l;
    return l;
  }


};

#endif
