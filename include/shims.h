#ifndef _SHIMS_H_
#define _SHIMS_H_

#include <string>
#include <cstring>

inline void assignBoundVar(std::string & a, const std::string & b) { a = b; }
inline void assignBoundVar(int & a, const std::string & b) { a = atoi(b.c_str()); }
inline void assignBoundVar(bool & a, const std::string & b) { a = atoi(b.c_str()) != 0; }
inline void assignBoundVar(float & a, const std::string & b) { a = atof(b.c_str()) != 0; }

inline void assignBoundVar(std::string & a, bool b) { a = std::to_string(b); }
inline void assignBoundVar(int & a, bool b) { a = int(b); }
inline void assignBoundVar(bool & a, bool b) { a = b; }
inline void assignBoundVar(float & a, bool b) { a = (int)b; }

#endif

