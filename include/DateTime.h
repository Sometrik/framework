#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <cstdlib>
#include <ctime>
#include <string>

class DateTime {
 public:
  
  DateTime();
  DateTime(double t, const char * timezone_text = 0);
  DateTime(time_t t, const char * timezone_text = 0);
  DateTime(int _year, int _month, int _day, int _hour, int _min, int _sec);
  DateTime(const char * s);
  DateTime(const std::string & s);

  bool parseRSSTime(const std::string & s);
  bool parseHTTPTime(const std::string & s);
  bool parseISOTime(const std::string & s);
  
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
  void setTime(time_t t);
  void setTimeUTC(time_t t);
  void setTime(const std::string & s);

  time_t getTime() const; // broken
  time_t getTimeUTC() const;  

  std::string toString() const;
  std::string dateToString() const;
  std::string toISOString() const;
  std::string toISOString2() const;

  static int get_timezone_offset();
  static struct tm * Localtime(const time_t * timep, struct tm * result);
  static struct tm * Gmtime(const time_t * timep, struct tm * result);
  static std::string getCurrentTimeString();
  static double getCurrentTime() const;
  
 private:
  short year, month, day, hour, min, sec;
  short tz_offset, day_of_week;
};

#endif
