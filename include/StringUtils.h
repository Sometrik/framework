#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

#include <vector>
#include <list>
#include <set>
#include <string>

#include <stdint.h>

class StringUtils {
 public:
  static const char * WHITESPACE;
  static const char * PUNCTUATION;

  static void trim(std::string & s);
  static void trimPunctuation(std::string & s);
  static void trimUtf8(std::string & s);
  static std::pair<std::string, std::string> splitCamelCase(const std::string & input);

  static std::vector<std::string> split(const std::string & line, char delimiter = 0);
  static std::vector<std::string> split(const std::string & line, const std::string & delimiter);
  static std::pair<std::string, std::string> splitOnce(const std::string & line, char delimiter = 0);
  static size_t split(const std::string & line, std::back_insert_iterator< std::vector<std::string> > backiter, char delimiter = 0);
  static size_t split(const std::string & line, std::back_insert_iterator< std::list<std::string> > backiter, char delimiter = 0);
  static size_t split(const std::string & input, std::back_insert_iterator< std::vector<std::string> > backiter, const std::string & delimiter);

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
  static bool isWord(const std::string & str);
  static bool isUpper(const std::string & s);
  static bool isBlank(const std::string & s);

  static bool hasWordCharacter(const std::string & input);

  static size_t length(const std::string & str);
  static std::string reverse(const std::string & input);

  static bool isValidUTF8(const std::string & input);

  static std::string toLower(std::string_view str);
  static std::string toUpper(std::string_view str);
  static std::string extractAscii(const std::string & s);
  static std::string extractNumber(const std::string & s);
  // static std::vector<std::string> split(const char * s, char delimiter = 0);
  static std::string decodeEntities(const std::string & s);
  static std::vector<std::string> extractHashtags(const std::string & s);

  static void trim(std::list<std::string> & l) {
    while (!l.empty() && l.front().empty()) l.pop_front();
    while (!l.empty() && l.back().empty()) l.pop_back();
  }
  
  // static std::string encodeIDN(const std::string & input);
  static std::string encodePunycode(const std::string & input);

  static std::string removeRepeatingLetters(const std::string & input);

  static std::vector<uint32_t> toVector(const std::string & input);
  static std::string fromVector(const std::vector<uint32_t> & input);

  static std::string createNonce(unsigned int size);

  static std::string substr(const std::string & input, size_t i, size_t l);

  static bool normalizeWhitespace(const std::string & input, std::string & output);
  static std::string normalize(const std::string & input);
  static std::string normalizeNFC(const std::string & input);

  static void repairXML(std::string & content);

  static std::string createRegex(std::string keyword);
};

#endif
