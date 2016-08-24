#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>

#include <stdint.h>

class StringUtils {
 public:
  static const char * WHITESPACE;
  static const char * PUNCTUATION;

  static void initialize();

  static void trim(std::string & s);
  static void trimPunctuation(std::string & s);
  static void trimUtf8(std::string & s);
  static std::vector<std::string> split(const std::string & line, char delimiter = 0);
  static std::pair<std::string, std::string> splitOnce(const std::string & line, char delimiter = 0);
  static size_t split(const std::string & line, std::back_insert_iterator< std::vector<std::string> > backiter, char delimiter = 0);
  static size_t split(const std::string & line, std::back_insert_iterator< std::list<std::string> > backiter, char delimiter = 0);
  static std::string join(const std::string & delimiter, const std::vector<std::string> & list);
  static std::string join(const char delimiter, const std::vector<std::string> & v);
  static std::string join(const char delimiter, const std::list<std::string> & l);
  static std::string join(const char delimiter, const std::set<std::string> & v);

  static void strip(std::string & s, char c);

  static std::string hexencode(const unsigned char * ptr, size_t len);
  static std::string hexencode(const std::basic_string<unsigned char> & data) { return hexencode(data.data(), data.size()); }

  static std::string quote(const std::string & str);
  static unsigned int chomp(char * str);
  static unsigned int chomp(std::string & str);
  static bool isNumber(const char * str);
  static bool isNumber(const std::string & str) { return isNumber(str.c_str()); }
  static bool isBlank(const char * str);
  static bool isBlank(const std::string & str) { return isBlank(str.c_str()); }
  static bool isBlank(uint32_t c );
  static bool isWord(const std::string & str);
  static uint32_t toLower(uint32_t c);
  static uint32_t toUpper(uint32_t c);

  static bool isDigit(uint32_t c);

  static bool isLower(uint32_t c);

  static bool isUpper(uint32_t c);
  static bool isUpper(const std::string & s);
  
  static std::string toLower(const std::string & str);
  static std::string toUpper(const std::string & str);
  static std::string extractAscii(const std::string & s);
  static std::string extractNumber(const std::string & s);
  // static std::vector<std::string> split(const char * s, char delimiter = 0);
  static std::string decodeEntities(const std::string & s);
  static std::string stripHTML(const std::string & s, bool strip_html = true); // and decode entities;

  static void trim(std::list<std::string> & l) {
    while (!l.empty() && l.front().empty()) l.pop_front();
    while (!l.empty() && l.back().empty()) l.pop_back();
  }
  
  static bool convertEntity(const std::string & entity, std::string & r); // html entities

  static std::set<std::string> extractLinks(const std::string & message, bool allow_utf8, bool extract_mentions, bool extract_hashtags);  // twitter entities

  // static std::string encodeIDN(const std::string & input);
  static std::string encodePunycode(const std::string & input);

  static std::string repairUTF8(const std::string & s);

  static bool isAlpha(uint32_t cp);
  static bool isIdeograph(uint32_t cp);
  static bool isSyllabary(uint32_t cp);
  static bool isArabic(uint32_t cp);
  static bool isComplexSymbol(uint32_t cp);
  static bool isSymbol(uint32_t cp);
  static bool isSpace(uint32_t cp);
  static bool isCombining(uint32_t cp);
  static bool isPrivate(uint32_t cp);

  static std::string removeRepeatingLetters(const std::string & input);

  static std::vector<uint32_t> toVector(const std::string & input);
  static std::string fromVector(const std::vector<uint32_t> & input);

  static std::string createNonce(unsigned int size);

  static std::string substr(const std::string & input, size_t i, size_t l);

 private:
  static std::map<std::string, int> entities;
  static bool initialized;
};

#endif
