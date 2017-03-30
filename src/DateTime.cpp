#include "DateTime.h"

#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstring>
#include <iostream>

#include <sys/time.h>

#include "StringUtils.h"

#ifdef WIN32
#include "win32/strptime.h"
#endif

using namespace std;

#if 0
extern char * tzname[2];
extern long timezone;
extern int daylight;
#endif

static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

struct tm *
DateTime::Localtime(const time_t * timep, struct tm * result) {
  assert(result);
#ifdef _MSC_VER
  localtime_s(result, timep); // returns errno
  return result;
#else
  return localtime_r(timep, result);
#endif
}

struct tm *
DateTime::Gmtime(const time_t * timep, struct tm * result) {
  assert(timep);
  assert(result);
#ifdef _MSC_VER
  gmtime_s(result, timep); // returns errno
  return result;
#else
  return gmtime_r(timep, result);
#endif
}

DateTime::DateTime() : year(0), month(0), day(0), hour(0), min(0), sec(0), tz_offset(-30000), day_of_week(-1)
{
  
}

DateTime::DateTime(double _t, const char * timezone_text) {
  setTime((time_t)_t, timezone_text);
}

DateTime::DateTime(time_t _t, const char * timezone_text) {
  setTime(_t, timezone_text);
}

DateTime::DateTime(int _year, int _month, int _day, int _hour, int _min, int _sec)
  : year(_year),
    month(_month),
    day(_day),
    hour(_hour),
    min(_min),
    sec(_sec),
    tz_offset(-30000),
    day_of_week(-1)
{
  
}

// 2011-06-03T09:20:53+0000
// 2012-11-21T16:08:42.000Z
// Tue May 22 09:42:26 -0500 2012
DateTime::DateTime(const char * _s)
{
  string s(_s);
  assert(s.size());
  setTime(s);
}

DateTime::DateTime(const string & s) {
  assert(s.size());
  setTime(s);
}

bool
DateTime::parseRSSTime(const string & s) {
  //  Wed, 17 Dec 2014 09:07:50 EST
  // "Mon, 01 Jan 1900 00:00:00 +0200"
  // 19 Dec 2014 10:35:29 +0200

  char s_weekday[5], s_month[5];
  s_weekday[0] = s_month[0] = 0;

  tz_offset = day_of_week = 0;  
  
  int yy, dd, hh, mm, ss;
  char tz_text[16];
  bool is_valid = false;
#ifdef WIN32
  is_valid = _snscanf(s.c_str(), s.size(), "%s %d %s %d %d:%d:%d %s", s_weekday, &dd, s_month, &yy, &hh, &mm, &ss, tz_text) == 8;
#else
  is_valid = sscanf(s.c_str(), "%s %d %s %d %d:%d:%d %s", s_weekday, &dd, s_month, &yy, &hh, &mm, &ss, tz_text) == 8;
#endif
  if (!is_valid) {
#ifdef WIN32
    is_valid = _snscanf(s.c_str(), s.size(), "%d %s %d %d:%d:%d %s", &dd, s_month, &yy, &hh, &mm, &ss, tz_text) == 7;
#else
    is_valid = sscanf(s.c_str(), "%d %s %d %d:%d:%d %s", &dd, s_month, &yy, &hh, &mm, &ss, tz_text) == 7;
#endif  
  }
    if (is_valid) {
    if (StringUtils::isNumber(tz_text)) {
      int tz = atoi(tz_text);
      tz_offset = int(tz / 100) * 60; // minutes?
    } else {
      // parse EST
      tz_offset = 0;
    }
    year = yy; day = dd; hour = hh; min = mm; sec = ss;
    const char * r = strstr(month_names, s_month);
    if (r) {
      month = 1 + (r - month_names) / 3;
      return true;
    }
  }

  year = month = day = hour = min = sec = 0;
  
  return false;  
}

bool
DateTime::parseHTTPTime(const string & s) {
  char s_weekday[5], s_month[5];
  s_weekday[0] = s_month[0] = 0;

  tz_offset = day_of_week = 0;  

  // Wed Aug 27 13:08:45 +0000 2008
  int yy, dd, hh, mm, ss, tz;
#ifdef WIN32
  int rv = _snscanf(s.c_str(), s.size(), "%s %s %d %d:%d:%d %d %d", s_weekday, s_month, &dd, &hh, &mm, &ss, &tz, &yy);
#else
  int rv = sscanf(s.c_str(), "%s %s %d %d:%d:%d %d %d", s_weekday, s_month, &dd, &hh, &mm, &ss, &tz, &yy);
#endif

  if (rv == 8) {
    tz_offset = int(tz / 100) * 60; // minutes?
    year = yy; day = dd; hour = hh; min = mm; sec = ss;
    const char * r = strstr(month_names, s_month);
    if (r) {
      month = 1 + (r - month_names) / 3;
      return true;
    }
  }

  year = month = day = hour = min = sec = 0;

  return false;
}

bool
DateTime::parseISOTime(const string & s) {
  tz_offset = day_of_week = 0;  

  // 2011-10-06T17:59:06.000Z
  int yy, mo, dd, hh, mi, ss;
#ifdef WIN32
  int rv = _snscanf(s.c_str(), s.size(), "%d-%d-%dT%d:%d:%d", &yy, &mo, &dd, &hh, &mi, &ss);
#else
  int rv = sscanf(s.c_str(), "%d-%d-%dT%d:%d:%d", &yy, &mo, &dd, &hh, &mi, &ss);
#endif
  if (rv != 6) {
#ifdef WIN32
    rv = _snscanf(s.c_str(), s.size(), "%d-%d-%d %d:%d:%d", &yy, &mo, &dd, &hh, &mi, &ss);
#else
    rv = sscanf(s.c_str(), "%d-%d-%d %d:%d:%d", &yy, &mo, &dd, &hh, &mi, &ss);
#endif
  }
  if (rv == 6) {
    year = yy; month = mo; day = dd; hour = hh; min = mi; sec = ss;
    return true;
  } else {
    year = month = day = hour = min = sec = 0;
    return false;
  }
}

void
DateTime::setTime(const string & s) {
  assert(s.size());
#ifdef WIN32
  year = month = day = hour = min = sec = 0;
#else
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));
  if (!strptime(s.c_str(), "%a %b %d %H:%M:%S %z %Y", &tm) &&
      !strptime(s.c_str(), "%Y-%m-%d %H:%M:%S", &tm) &&
      !strptime(s.c_str(), "%Y-%m-%dT%H:%M:%S%z", &tm) &&
      !strptime(s.c_str(), "%Y-%m-%dT%H:%M:%S.000%z", &tm) &&
      !strptime(s.c_str(), "%Y-%m-%dT%H:%M:%S.000Z", &tm) &&
      !strptime(s.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm) &&
      !strptime(s.c_str(), "%Y-%m-%d", &tm) &&
      !strptime(s.c_str(), "%d.%m.%Y %H:%M", &tm) &&
      !strptime(s.c_str(), "%d.%m.%Y", &tm) &&
      !strptime(s.c_str(), "%d.%m.%Y, %H:%M", &tm) &&
      !strptime(s.c_str(), "%a, %d %b %Y %H:%M:%S %z", &tm) &&
      !strptime(s.c_str(), "%a, %d %b %Y %H:%M %z", &tm) &&
      !strptime(s.c_str(), "%a, %d %b %Y %H:%M:%S %Z", &tm) &&
      !strptime(s.c_str(), "%d %b %Y %H:%M:%S %z", &tm) &&
      !strptime(s.c_str(), "%Y/%m/%d %H:%M:%S %z", &tm)
      ) {
    // no milliseconds :(
    // Sun, 01 Sep 2013 10:12:00 +0300
    // 02 Sep 2013 05:00:00 +0300
    cerr << "failed to parse time string \"" << s << "\"\n";
    assert(0);
  }

  // Sun, 01 Sep 2013 10:12:00 +0300
    
  year = 1900 + tm.tm_year;
  month = 1 + tm.tm_mon;
  day = tm.tm_mday;
  hour = tm.tm_hour;
  min = tm.tm_min;
  sec = tm.tm_sec;
#ifdef WIN32
  tz_offset = 0; // FIX
#else
  tz_offset = tm.tm_gmtoff / 60;
#endif
  day_of_week = (tm.tm_wday + 6) % 7 + 1; 

  // assert(year >= 1964);

  // cerr << "parsed time from " << s << " => " << getTime() << " (tz = " << tz_hours << ")\n";
#endif
}

void
DateTime::setTime(time_t t, const char * timezone_text) {
  if (timezone_text) {
#ifdef _WIN32
    setTime(t);
#else
    char s[256] = "TZ=";
    char s2[] = "TZ";
    strncat(s + 3, timezone_text, 255 - 3);
    putenv(s);
    tzset();
    setTime(t);
    putenv(s2);
    tzset();
#endif
  } else {
    setTime(t);
  }
}

void
DateTime::setTime(time_t t) {
  tm ts;
  if (Localtime(&t, &ts)) {
    year = ts.tm_year + 1900;
    month = ts.tm_mon + 1;
    day = ts.tm_mday;
    hour = ts.tm_hour;
    min = ts.tm_min;
    sec = ts.tm_sec;
    day_of_week = (ts.tm_wday + 6) % 7 + 1; 
  } else {
    year = month = day = hour = min = sec = 0;    
    day_of_week = -1;
  }
}

void
DateTime::setTimeUTC(time_t t) {
  tm ts;
  if (Gmtime(&t, &ts)) {
    year = ts.tm_year + 1900;
    month = ts.tm_mon + 1;
    day = ts.tm_mday;
    hour = ts.tm_hour;
    min = ts.tm_min;
    sec = ts.tm_sec;
    day_of_week = (ts.tm_wday + 6) % 7 + 1; 
  } else {
    year = month = day = hour = min = sec = 0;    
    day_of_week = -1;
  }
}

time_t
DateTime::getTime() const {
  if (tz_offset != -30000) {
    return getTimeUTC() - tz_offset * 60;
  } else {
    tm tt;
    tt.tm_sec = sec;
    tt.tm_min = min;
    tt.tm_hour = hour;
    tt.tm_mday = day;
    tt.tm_mon = month - 1;
    tt.tm_year = year - 1900;
    tt.tm_wday = 0;
    tt.tm_yday = 0;
    tt.tm_isdst = -1; // n/a (is this right)
#ifndef WIN32
    tt.tm_gmtoff = 0;
#endif
    return mktime(&tt);
  }
}

time_t
DateTime::getTimeUTC() const {
  tm tt;
  tt.tm_sec = sec;
  tt.tm_min = min;
  tt.tm_hour = hour;
  tt.tm_mday = day;
  tt.tm_mon = month - 1;
  tt.tm_year = year - 1900;
  tt.tm_wday = 0;
  tt.tm_yday = 0;
  tt.tm_isdst = -1;
#if defined WIN32
  return _mkgmtime(&tt);
#elif defined __ANDROID__
  //this needs to be changed
  return mktime(&tt);
#else
  return timegm(&tt);
#endif
}

int
DateTime::get_timezone_offset() {
#ifdef _WIN32
  return _timezone;
#else
  return timezone;
#endif
}

string
DateTime::toString() const {
  char s[256];
  snprintf(s, 255, "%d.%d.%d %02d:%02d:%02d", day, month, year, hour, min, sec);
  return string(s);
}

string
DateTime::dateToString() const {
  char s[256];
  snprintf(s, 255, "%d.%d.%d", day, month, year);
  return string(s);
}

string
DateTime::toISOString() const {
  char s[256];
  snprintf(s, 255, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
  return string(s);
}

string
DateTime::toISOString2() const {
  char s[256];
  snprintf(s, 255, "%04d-%02d-%02dT%02d:%02d:%02dZ", year, month, day, hour, min, sec);
  return string(s);
}

string
DateTime::getCurrentTimeString() {
  DateTime t;
  t.setTime(time(0));
  return t.toString();
}

DateTime &
DateTime::normalize() {
  tm tt;
  tt.tm_sec = sec;
  tt.tm_min = min;
  tt.tm_hour = hour;
  tt.tm_mday = day;
  tt.tm_mon = month - 1;
  tt.tm_year = year - 1900;
  tt.tm_wday = 0;
  tt.tm_yday = 0;
  tt.tm_isdst = -1;  
  mktime(&tt);
  sec = tt.tm_sec;
  min = tt.tm_min;
  hour = tt.tm_hour;
  day = tt.tm_mday;
  month = tt.tm_mon + 1;
  year = tt.tm_year + 1900;  
  return *this;
}

double
DateTime::getCurrentTime() {
  struct timeval tv;
  struct timezone tz;
  int r = gettimeofday(&tv, &tz);
  double t = 0;
  if (r == 0) {
    t = (double)tv.tv_sec + tv.tv_usec / 1000000.0;
  }
  return t;
}
