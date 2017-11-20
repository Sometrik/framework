#ifndef _ANDROID_COMPATABILITY_H_
#define _ANDROID_COMPATABILITY_H_

#include <string>
#include <sstream>
#include <cstdlib>

namespace std {
#if 0
  inline string to_string(long long value) {
    if (value == 0) {
      return "0";
    } else {
      char buffer[32];
      char * ptr = &buffer[0];

      unsigned int count = 0;
      if (value < 0) {
        value = -value;
        buffer[0] = '-';
        count++;
      }

      long long tmp = value;
      while (tmp > 0) {
        tmp = tmp / 10;
        count++;
      }

      ptr += count;

      *ptr = '\0';

      long long t = 0, res = 0;
      do {
        res = value - 10 * (t = value / 10);
        * -- ptr = '0' + res;
      } while ((value = t) != 0);

      return buffer;
    }
  }

  inline string to_string(int value) {
    char str[32];
    snprintf(str, 32, "%d", value);
    return str;
  }
  inline string to_string(size_t value) {
    return to_string((long long)value);
  }
  inline string to_string(time_t value) {
    return to_string((long long)value);
  }
#else
  template <typename T>
    inline string to_string(T value) {
    ostringstream os;
    os << value;
    return os.str();
  }
#endif

  inline int stoi(const string & str, size_t* idx = 0, int base = 10) {
    return strtoul(str.c_str(), 0, base);
  }

  inline float stof(const string & value) {
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

#if 0
  inline unsigned long long stoull(const string & value) {
     stringstream buffer(value);
     unsigned long long l;
     buffer >> l;
     return l;
   }
#endif

  inline double stod(const string & value) {
    stringstream buffer(value);
    double d;
    buffer >> d;
    return d;
  }

#if 0
  inline long long stoll(const std::string & input) {
    const char * str = input.c_str();
    long long result = 0;
    long long sign = 1;

    result = 0;

    while (('-' == (*str)) || ((*str) == '+')) {
      if (*str == '-') sign *= -1;
      str++;
    }

    while ((*str >= '0') && (*str <= '9')) {
      result = (result * 10) + ((*str) - '0');
      str++;
    }

    return result * sign;
  }
#else
  inline long long stoll(const string & value) {
    stringstream buffer(value);
    long long l;
    buffer >> l;
    return l;
  }
#endif

};

#endif
