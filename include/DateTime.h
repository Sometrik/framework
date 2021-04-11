#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <cstdlib>
#include <ctime>
#include <string>

static inline int leap_year(int year) {
  int yy;
  return (((year & 0x03) == 0) &&
	  ((((yy = year / 100) * 100) != year) or
	   ((yy & 0x03) == 0)));
}

static inline int Year_to_Days(int year) {
  int days = year * 365;
  year = year >> 2;
  days += year;
  year = year / 25;
  days -= year;
  days += year >> 2;
  return days;
}

static inline int Days_in_Year(int leap, int month) {
  if (leap == 0) {
    switch (month) {
    case 1: return 0;
    case 2: return 31;
    case 3: return 59;
    case 4: return 90;
    case 5: return 120;
    case 6: return 151;
    case 7: return 181;
    case 8: return 212;
    case 9: return 243;
    case 10: return 273;
    case 11: return 304;
    case 12: return 334;
    }
  } else {
    switch (month) {
    case 1: return 0;
    case 2: return 31;
    case 3: return 60;
    case 4: return 91;
    case 5: return 121;
    case 6: return 152;
    case 7: return 182;
    case 8: return 213;
    case 9: return 244;
    case 10: return 274;
    case 11: return 305;
    case 12: return 335;
    }
  }
  return 0;
}

static inline int Date_to_Days(int year, int month, int day) {
  if ((year >= 1) &&
      (month >= 1) && (month <= 12) &&
      (day >= 1)) {
    int leap = leap_year(year);
    // ($day   <= $DateCalc_Days_in_Month_[($leap=DateCalc_leap_year($year))][$month]))
    return Year_to_Days(--year) + Days_in_Year(leap, month) + day;
  }
  return 0;
}

class DateTime {
 public:
  enum TimezoneSelector {
    UTC = 1,
    LOCAL
  };
  
  DateTime();
  DateTime(int _year, int _month, int _day, int _hour, int _min, int _sec, TimezoneSelector tz_sel);
  DateTime(const char * s);
  DateTime(const std::string & s);

  DateTime & parseTime(const std::string & s) {
    parseISOTime(s);
    if (!isValid()) parseRSSTime(s);
    if (!isValid()) parseISODate(s);
    return *this;
  }
  
  bool parseRSSTime(const std::string & s);
  bool parseHTTPTime(const std::string & s);
  DateTime & parseISOTime(const std::string & s);
  bool parseISODate(const std::string & s);
  
  int getYear() const { return year; }
  int getMonth() const { return month; }
  int getDay() const { return day; }
  int getHour() const { return hour; }
  int getMin() const { return min; }
  int getSec() const { return sec; }  
  int getDayOfWeek() const { return day_of_week; }

  DateTime & setYear(short _year) { year = _year; return *this; }
  DateTime & setMonth(short _month) { month = _month; return *this; }
  DateTime & setDay(short _day) { day = _day; return *this; }
  DateTime & setHour(short _hour) { hour = _hour; return *this; }
  DateTime & setMin(short _min) { min = _min; return *this; }
  DateTime & setSec(short _sec) { sec = _sec; return *this; }

  // day can overflow before normalization
  DateTime & addDeltaDays(short days) { day = short(day + days); return *this; }

  DateTime & normalize();

  void setTime(time_t t, const char * timezone);
  void setTimeLocal(time_t t);
  void setTimeUTC(time_t t);
  void setTime(const std::string & s);

  time_t getUnixTime() const; // applies timezone

  std::string toString() const;
  std::string dateToString() const;
  std::string toISOStringSimple() const;
  std::string toISOStringTZ() const;

  static int get_timezone_offset();
  static struct tm * Localtime(const time_t * timep, struct tm * result);
  static struct tm * Gmtime(const time_t * timep, struct tm * result);
  static std::string getCurrentTimeString();
  static double getCurrentTime();

  static long long now();

  bool isValid() const { return is_valid; }

  static inline int getDaynum(time_t t) {
    struct tm td;
    localtime_r(&t, &td);
    return Date_to_Days(1900 + td.tm_year, 1 + td.tm_mon, td.tm_mday);
  }
  
 private:
  short year, month, day, hour, min, sec;
  short tz_offset, day_of_week;
  bool is_valid;
};

#endif
