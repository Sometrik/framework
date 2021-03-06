#include "DateTime.h"

#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstring>

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

#ifndef WIN32
#define _snscanf(input, size, format, ...) sscanf(input, format, __VA_ARGS__)
#endif

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

DateTime::DateTime() : year(0), month(0), day(0), hour(0), min(0), sec(0), tz_offset(-30000), day_of_week(-1), is_valid(false)
{
  
}

DateTime::DateTime(int _year, int _month, int _day, int _hour, int _min, int _sec, TimezoneSelector tz_sel)
  : year(_year),
    month(_month),
    day(_day),
    hour(_hour),
    min(_min),
    sec(_sec),
    tz_offset(tz_sel == UTC ? 0 : -30000),
    day_of_week(-1),
    is_valid(true)
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
  if (s.size() >= 64) return false;

  //  Wed, 17 Dec 2014 09:07:50 EST
  // "Mon, 01 Jan 1900 00:00:00 +0200"
  // 19 Dec 2014 10:35:29 +0200

  char s_weekday[64], s_month[64];
  s_weekday[0] = s_month[0] = 0;

  tz_offset = day_of_week = 0;  
  
  int yy, dd, hh, mm, ss;
  char tz_text[32];
  bool success = _snscanf(s.c_str(), s.size(), "%s %d %s %d %d:%d:%d %s", s_weekday, &dd, s_month, &yy, &hh, &mm, &ss, tz_text) == 8;
  if (!success) {
    success = _snscanf(s.c_str(), s.size(), "%d %s %d %d:%d:%d %s", &dd, s_month, &yy, &hh, &mm, &ss, tz_text) == 7;
  }
  if (success) {
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
      is_valid = true;
      return true;
    }
  }

  year = month = day = hour = min = sec = 0;
  is_valid = false;
  
  return false;  
}

bool
DateTime::parseHTTPTime(const string & s) {
  if (s.size() >= 64) return false;

  char s_weekday[64], s_month[64];
  s_weekday[0] = s_month[0] = 0;

  tz_offset = day_of_week = 0;  

  // Wed Aug 27 13:08:45 +0000 2008
  int yy, dd, hh, mm, ss, tz;
  int rv = _snscanf(s.c_str(), s.size(), "%s %s %d %d:%d:%d %d %d", s_weekday, s_month, &dd, &hh, &mm, &ss, &tz, &yy);

  if (rv == 8) {
    tz_offset = int(tz / 100) * 60; // minutes?
    year = yy; day = dd; hour = hh; min = mm; sec = ss;
    const char * r = strstr(month_names, s_month);
    if (r) {
      month = 1 + (r - month_names) / 3;
      is_valid = true;
      return true;
    }
  }

  year = month = day = hour = min = sec = 0;
  is_valid = false;
  
  return false;
}

DateTime &
DateTime::parseISOTime(const string & s) {
  tz_offset = day_of_week = 0;  

  // 2011-10-06T17:59:06.000Z
  // 2013-03-05T22:12:37+02:00
  int yy, mo, dd, hh, mi, ss, timezone = 0;
  char delimiter;  
  int rv = _snscanf(s.c_str(), s.size(), "%04d-%02d-%02d%c%02d:%02d:%02d", &yy, &mo, &dd, &delimiter, &hh, &mi, &ss);
  bool success = false;
  if (rv == 7 && (delimiter == ' ' || delimiter == 'T')) {
    size_t pos = 19;
    if (pos < s.size() && s[pos] == '.') pos += 4;
    if (pos + 5 < s.size() && (s[pos] == '+' || s[pos] == '-')) {
      int sign = s[pos] == '+' ? 1 : -1;
      pos++;
      int tz_hour, tz_min;
      int rv2 = _snscanf(s.c_str() + pos, s.size() - pos, "%02d:%02d", &tz_hour, &tz_min);
      if (rv2 == 2) {
	timezone = tz_hour * 60 + tz_min;
      }
    }
    success = true;
  } else {
    rv = _snscanf(s.c_str(), s.size(), "%04d%02d%02d-%02d%02d%02d", &yy, &mo, &dd, &hh, &mi, &ss);
    success = rv == 6;
  }
  is_valid = success;
  if (success) {
    year = yy; month = mo; day = dd; hour = hh; min = mi; sec = ss; tz_offset = timezone;
  } else {
    year = month = day = hour = min = sec = 0;
  }
  return *this;
}

bool
DateTime::parseISODate(const string & s) {
  tz_offset = day_of_week = 0;  

  int yy, mo, dd;
  int rv = _snscanf(s.c_str(), s.size(), "%d-%d-%d", &yy, &mo, &dd);
  if (rv == 3) {
    year = yy; month = mo; day = dd; hour = 0; min = 0; sec = 0;
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
    // cerr << "failed to parse time string \"" << s << "\"\n";
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
  is_valid = true;
  
  // assert(year >= 1964);

  // cerr << "parsed time from " << s << " => " << getTime() << " (tz = " << tz_hours << ")\n";
#endif
}

void
DateTime::setTime(time_t t, const char * timezone_text) {
  if (timezone_text) {
#ifdef _WIN32
    setTimeLocal(t);
#else
    char s[256] = "TZ=";
    char s2[] = "TZ";
    strncat(s + 3, timezone_text, 255 - 3);
    putenv(s);
    tzset();
    setTimeLocal(t);
    putenv(s2);
    tzset();
#endif
  } else {
    setTimeLocal(t);
  }
}

void
DateTime::setTimeLocal(time_t t) {
  tm ts;
  if (Localtime(&t, &ts)) {
    year = ts.tm_year + 1900;
    month = ts.tm_mon + 1;
    day = ts.tm_mday;
    hour = ts.tm_hour;
    min = ts.tm_min;
    sec = ts.tm_sec;
    day_of_week = (ts.tm_wday + 6) % 7 + 1;
    tz_offset = get_timezone_offset();
    is_valid = true;
  } else {
    year = month = day = hour = min = sec = 0;    
    day_of_week = -1;
    tz_offset = 0;
    is_valid = false;
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
    tz_offset = 0;
    is_valid = true;
  } else {
    year = month = day = hour = min = sec = 0;    
    day_of_week = -1;
    tz_offset = 0;
    is_valid = false;
  }
}

#ifdef __ANDROID__
static time_t my_timegm(struct tm * t)
/* struct tm to seconds since Unix epoch */
{
    long year;
    time_t result;
#define MONTHSPERYEAR   12      /* months per calendar year */
    static const int cumdays[MONTHSPERYEAR] =
        { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    /*@ +matchanyintegral @*/
    year = 1900 + t->tm_year + t->tm_mon / MONTHSPERYEAR;
    result = (year - 1970) * 365 + cumdays[t->tm_mon % MONTHSPERYEAR];
    result += (year - 1968) / 4;
    result -= (year - 1900) / 100;
    result += (year - 1600) / 400;
    if ((year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0) &&
        (t->tm_mon % MONTHSPERYEAR) < 2)
        result--;
    result += t->tm_mday - 1;
    result *= 24;
    result += t->tm_hour;
    result *= 60;
    result += t->tm_min;
    result *= 60;
    result += t->tm_sec;
    if (t->tm_isdst == 1)
        result -= 3600;
    /*@ -matchanyintegral @*/
    return (result);
}
#endif

time_t
DateTime::getUnixTime() const {
  tm tt;
  tt.tm_sec = sec;
  tt.tm_min = min;
  tt.tm_hour = hour;
  tt.tm_mday = day;
  tt.tm_mon = month - 1;
  tt.tm_year = year - 1900;
  tt.tm_wday = 0;
  tt.tm_yday = 0;
  tt.tm_isdst = -1; // ?
#ifndef WIN32
  tt.tm_gmtoff = 0;
#endif

  if (tz_offset == -30000) { // unknown timezone, use localtime
    return mktime(&tt);
  } else {
#if defined WIN32
    time_t t = _mkgmtime(&tt);
#elif defined __ANDROID__
    time_t t = my_timegm(&tt);
#else
    time_t t = timegm(&tt);
#endif
    return t - tz_offset * 60;
  }
}

int
DateTime::get_timezone_offset() {
#ifdef _WIN32
  return _timezone / 60;
#else
  return timezone / 60;
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
DateTime::toISOStringSimple() const {
  char s[256];
  snprintf(s, 255, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
  return string(s);
}

string
DateTime::toISOStringTZ() const {
  char s[256];
  snprintf(s, 255, "%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour, min, sec);
  string r(s);
  if (tz_offset == 0) r += 'Z';
  else if (tz_offset != -30000) {
    snprintf(s, 255, "%c%02d:%02d", tz_offset < 0 ? '-' : '+', abs(tz_offset / 60), abs(tz_offset % 60));
    r += string(s);
  }
  return r;
}

string
DateTime::getCurrentTimeString() {
  DateTime t;
  t.setTimeLocal(now() / 1000);
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
  int r = gettimeofday(&tv, 0);
  if (r == 0) {
    return (double)tv.tv_sec + tv.tv_usec / 1000000.0;
  } else {
    return 0;
  }
}

long long
DateTime::now() {
  struct timeval tv;
  int r = gettimeofday(&tv, 0);
  if (r == 0) {
    return (long long)1000 * tv.tv_sec + tv.tv_usec / 1000;
  } else {
    return 0;
  }
}
