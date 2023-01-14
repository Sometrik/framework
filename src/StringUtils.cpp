#include <StringUtils.h>

#include <cstring>
#include <cctype>
#include <algorithm>
#include <cassert>

#include <character_tests.h>

#include <utf8.h>
#include <utf8proc.h>

#include <iostream>

using namespace std;

const char * StringUtils::WHITESPACE = " \t\b\x0a\x0d";
const char * StringUtils::PUNCTUATION = ",.;?!-*:_/\\()[]{}";

void
StringUtils::trim(string & s) {
  string::size_type not_white;
  
  // trim leading whitespace
  not_white = s.find_first_not_of(WHITESPACE);
  if (not_white != string::npos) s.erase(0, not_white);
  
  // trim trailing space
  not_white = s.find_last_not_of(WHITESPACE);
  if (not_white == string::npos) {
    s.clear();
  } else {
    s.erase(not_white + 1);
  }
}

void
StringUtils::trimPunctuation(string & input) {
#if 0
  string::size_type not_punct;
  
  // trim leading whitespace
  not_punct = s.find_first_not_of(PUNCTUATION);
  if (not_punct != string::npos) s.erase(0, not_punct);
  
  // trim trailing space
  not_punct = s.find_last_not_of(PUNCTUATION);
  if (not_punct == string::npos) {
    s.clear();
  } else {
    s.erase(not_punct + 1);
  }
#else
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();
  
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (is_alpha_unicode(c)) {
      utf8::prior(str_i, str);
      break;
    }
  }

  string tmp, part;

  auto tmp_inserter = back_inserter(tmp);
  auto part_inserter = back_inserter(part);

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (is_alpha_unicode(c)) {
      if (!part.empty()) {
	tmp += part;
	part.clear();
	tmp_inserter = back_inserter(tmp);
	part_inserter = back_inserter(part);
      }

      utf8::append(c, tmp_inserter);
    } else {
      utf8::append(c, part_inserter);
    }
  }
  
  input = tmp;
#endif
}

vector<string>
StringUtils::extractHashtags(const std::string & input) {
  vector<string> r;

  try {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size();

    string current_hashtag;
    auto output_inserter = back_inserter(current_hashtag);
    unsigned int state = 0;
    
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end);

      switch (state) {
      case 0:
	if (c == '#') state = 1;
	break;
      case 1:
	if (c == '_' || is_digit_unicode(c) || is_alpha_unicode(c) || is_syllabary_unicode(c) || is_ideograph_unicode(c)) utf8::append(c, output_inserter);
	else {
	  if (!current_hashtag.empty()) {
	    r.push_back(current_hashtag);
	    current_hashtag.clear();
	    output_inserter = back_inserter(current_hashtag);
	  }
	  if (c != '#') state = 0;
	}
	break;
      }
    }

    if (!current_hashtag.empty()) {
      r.push_back(current_hashtag);
    }
  } catch (exception & e) {
    cerr << "failed to extract hashtags: " << e.what() << endl;
  }

  return r;
}

bool
StringUtils::normalizeWhitespace(const string & input, string & output) {
  output.clear();
  
  try {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size();
    
    auto output_inserter = back_inserter(output);

    bool is_whitespace = false;
  
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end);
      if (is_control_unicode(c)) {
	// skip Zero Width Space and similar
      } else if (!is_space_unicode(c)) {
	if (is_whitespace) {
	  utf8::append(' ', output_inserter);
	  is_whitespace = false;
	}
	utf8::append(c, output_inserter);
      } else if (!is_whitespace && !output.empty()) {
	is_whitespace = true;
      }
    }
  } catch (exception & e) {
    cerr << "failed to normalize: " << input << endl;
    return false;
  }
  return true;
}

static inline uint32_t normalizeChar(const uint32_t c) {
  if (c >= 0x24b6 && c <= 0x24cf) return c - 0x24b6 + 'A'; // Circled Latin Capital Letter
  if (c >= 0x24d0 && c <= 0x24e9) return c - 0x24d0 + 'a'; // Circled Latin Small Letter
  if (c >= 0x01D400 && c <= 0x01D419) return c - 0x01D400 + 'A'; // Mathematical Bold Capital
  if (c >= 0x01D41A && c <= 0x01D433) return c - 0x01D41A + 'a'; // Mathematical Bold Small
  if (c >= 0x01D434 && c <= 0x01D44D) return c - 0x01D434 + 'A'; // Mathematical Italic Capital
  if (c >= 0x01D44E && c <= 0x01D467) return c - 0x01D44E + 'a'; // Mathematical Italic Small
  if (c >= 0x01D468 && c <= 0x01D481) return c - 0x01D468 + 'A'; // Mathematical Bold Italic Capital
  if (c >= 0x01D482 && c <= 0x01D49B) return c - 0x01D482 + 'a'; // Mathematical Bold Italic Small
  if (c >= 0x01D49C && c <= 0x01D4B5) return c - 0x01D49C + 'A'; // Mathematical Script Capital
  if (c >= 0x01D4B6 && c <= 0x01D4CF) return c - 0x01D4B6 + 'a'; // Mathematical Script Small
  if (c >= 0x01D4D0 && c <= 0x01D4E9) return c - 0x01D4D0 + 'A'; // Mathematical Bold Script Capital
  if (c >= 0x01D4EA && c <= 0x01D503) return c - 0x01D4EA + 'a'; // Mathematical Bold Script Small
  if (c >= 0x01D504 && c <= 0x01D51C) return c - 0x01D504 + 'A'; // Mathematical Fraktur Capital
  if (c >= 0x01D51E && c <= 0x01D537) return c - 0x01D51E + 'a'; // Mathematical Fraktur Small
  if (c >= 0x01D538 && c <= 0x01D551) return c - 0x01D538 + 'A'; // Mathematical Double-Struck Capital
  if (c >= 0x01D552 && c <= 0x01D56B) return c - 0x01D552 + 'a'; // Mathematical Double-Struck Small  
  if (c >= 0x01D56C && c <= 0x01D585) return c - 0x01D56C + 'A'; // Mathematical Bold Fraktur Capital
  if (c >= 0x01D586 && c <= 0x01D59F) return c - 0x01D586 + 'a'; // Mathematical Bold Fraktur Small
  if (c >= 0x01D5A0 && c <= 0x01D5B9) return c - 0x01D5A0 + 'A'; // Mathematical Sans-Serif Capital
  if (c >= 0x01D5BA && c <= 0x01D5D3) return c - 0x01D5BA + 'a'; // Mathematical Sans-Serif Small
  if (c >= 0x01D5D4 && c <= 0x01D5ED) return c - 0x01D5D4 + 'A'; // Mathematical Sans-Serif Bold Capital
  if (c >= 0x01D5EE && c <= 0x01D607) return c - 0x01D5EE + 'a'; // Mathematical Sans-Serif Bold Small
  if (c >= 0x01D608 && c <= 0x01D621) return c - 0x01D608 + 'A'; // Mathematical Sans-Serif Italic Capital
  if (c >= 0x01D622 && c <= 0x01D63B) return c - 0x01D622 + 'a'; // Mathematical Sans-Serif Italic Small
  if (c >= 0x01D63C && c <= 0x01D655) return c - 0x01D63C + 'A'; // Mathematical Sans-Serif Bold Italic Capital
  if (c >= 0x01D656 && c <= 0x01D66F) return c - 0x01D656 + 'a'; // Mathematical Sans-Serif Bold Italic Small
  if (c >= 0x01D670 && c <= 0x01D689) return c - 0x01D670 + 'A'; // Mathematical Monospace Capital
  if (c >= 0x01D68A && c <= 0x01D6A3) return c - 0x01D68A + 'a'; // Mathematical Monospace Small
  return c;
}

std::string
StringUtils::normalize(const std::string & input) {
  string output;
  
  try {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size();
    
    auto output_inserter = back_inserter(output);

    bool is_whitespace = false;
  
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end);
      if (is_control_unicode(c)) {
	// skip Zero Width Space and similar
      } else if (!is_space_unicode(c)) {
	if (is_whitespace) {
	  utf8::append(' ', output_inserter);
	  is_whitespace = false;
	}
	utf8::append(normalizeChar(c), output_inserter);
      } else if (!is_whitespace && !output.empty()) {
	is_whitespace = true;
      }
    }
  } catch (exception & e) {
    cerr << "failed to normalize: " << input << endl;
  }

  // cerr << "normalized " << input << " to " << output << endl;
  
  return output;
}

void
StringUtils::trimUtf8(string & s) {
  string n;
  int state = 1;
  string output, waiting_whitespace;

  const char * str = s.c_str();
  const char * str_i = str;
  const char * end = str + s.size();

  auto output_inserter = back_inserter(output);
  auto ww_inserter = back_inserter(waiting_whitespace);
  
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    assert(c);
    
    bool is_whitespace = is_space_unicode(c);

    switch (state) {
    case 1:
      if (!is_whitespace) {
	state++;
	utf8::append(c, output_inserter);
      }
      break;
    case 2:
      if (is_whitespace) {
	utf8::append(c, ww_inserter);
      } else {
	if (!waiting_whitespace.empty()) {
	  output += waiting_whitespace;
	  waiting_whitespace.clear();
	  output_inserter = back_inserter(output);
	  ww_inserter = back_inserter(waiting_whitespace);
	}
	utf8::append(c, output_inserter);
      }
      break;
    }
  }
  s = output;
}

void
StringUtils::strip(string & s, char c) {
  unsigned int pos = 0;
  for (unsigned int i = 0; i < s.size(); i++) {
    if (s[i] != c) {
      if (i != pos) s[pos] = s[i];
      pos++;
    }
  }
  s.erase(pos);
}

pair<string, string>
StringUtils::splitCamelCase(const std::string & input) {
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 

  string r1, r2;
  int state = 0;
  auto r1_inserter = back_inserter(r1);
  auto r2_inserter = back_inserter(r2);

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol

    if (state == 0 && is_alpha_unicode(c)) {
      utf8::append(c, r1_inserter);
      state++;
    } else if (state == 1 && is_lower_unicode(c)) {
      utf8::append(c, r1_inserter);
    } else {
      state = 2;
      utf8::append(c, r2_inserter);      
    }
  }

  return make_pair(r1, r2);
}

// empty string returns empty vector
// otherwise returns delimiter count + 1 strings
vector<string>
StringUtils::split(const string & line, char delimiter) {
  vector<string> v;
  // back_insert_iterator< vector<string> > backiter(v);
  split(line, back_inserter(v), delimiter);
  return v;
}

vector<string>
StringUtils::split(const std::string & line, const std::string & delimiter) {
  vector<string> v;
  split(line, back_inserter(v), delimiter);
  return v;
}

pair<string, string>
StringUtils::splitOnce(const string & line, char delimiter) {
  for (unsigned int i = 0, len = (unsigned int)line.size(); i < len; i++) {
    if ((delimiter == 0 && isspace(line[i])) || (delimiter != 0 && line[i] == delimiter)) {
      unsigned int i0 = i;
      if (delimiter == 0) {
	while (isspace(line[i + 1])) i++;
      }
      return pair<string, string>(line.substr(0, i0), line.substr(i + 1));
    }
  }
  return pair<string, string>(line, "");
}

size_t
StringUtils::split(const string & line, back_insert_iterator< vector<string> > backiter, char delimiter) {
  unsigned int n = 0;
  if (!line.empty()) {
    char * str = new char[line.size() + 1];
    strcpy(str, line.c_str());
    char * current = str;
    for (unsigned int i = 0, len = (int)line.size(); i < len; i++) {
      if ((delimiter == 0 && isspace(str[i])) || (delimiter != 0 && str[i] == delimiter)) {
	str[i] = 0;
	*backiter++ = current;
	n++;
	if (delimiter == 0) {
	  while (isspace(str[i + 1])) i++;
	}
	current = str + i + 1;
      }
    }
    *backiter++ = current;
    n++;
    delete[] str;
  }
  return n;
}

size_t
StringUtils::split(const string & line, back_insert_iterator< list<string> > backiter, char delimiter) {
  unsigned int n = 0;
  if (line.size()) {
    char * str = new char[line.size() + 1];
    strcpy(str, line.c_str());
    char * current = str;
    for (unsigned int i = 0, len = (int)line.size(); i < len; i++) {
      if ((delimiter == 0 && isspace(str[i])) || (delimiter != 0 && str[i] == delimiter)) {
	str[i] = 0;
	*backiter++ = current;
	n++;
	if (delimiter == 0) {
	  while (isspace(str[i + 1])) i++;
	}
	current = str + i + 1;
      }
    }
    *backiter++ = current;
    n++;
    delete[] str;
  }
  return n;
}

size_t
StringUtils::split(const string & input, back_insert_iterator< vector<string> > backiter, const std::string & delimiter) {
  unsigned int n = 0;
  for (string::size_type pos = 0; pos < input.size(); ) {
    auto pos2 = input.find(delimiter, pos);
    if (pos2 == string::npos) pos2 = input.size();
    *backiter++ = input.substr(pos, pos2 - pos);
    pos = pos2 + delimiter.size();
    n++;
  }
  return n;
}

string
StringUtils::join(const string & delimiter, const vector<string> & v) {
  string s;
  if (v.size()) {
    for (vector<string>::const_iterator it = v.begin(); it != v.end(); ++it) {
      if (it != v.begin()) s += delimiter;
      s += *it;
    }
  }
  return s;
}

string
StringUtils::join(const char delimiter, const vector<string> & v) {
  string s;
  if (v.size()) {
    for (vector<string>::const_iterator it = v.begin(); it != v.end(); ++it) {
      if (it != v.begin()) s += delimiter;
      s += *it;
    }
  }
  return s;
}

string
StringUtils::join(const char delimiter, const set<string> & v) {
  string s;
  if (!v.empty()) {
    for (auto it = v.begin(); it != v.end(); ++it) {
      if (it != v.begin()) s += delimiter;
      s += *it;
    }
  }
  return s;
}

string
StringUtils::join(const char delimiter, const list<string> & v) {
  string s;
  if (!v.empty()) {
    for (auto it = v.begin(); it != v.end(); ++it) {
      if (it != v.begin()) s += delimiter;
      s += *it;
    }
  }
  return s;
}

unsigned int
StringUtils::chomp(char * str) {
  int cnt = 0, len = (int)strlen(str);
  while (str[len - 1] == '\n' || str[len - 1] == '\r') {
    len--;
    cnt++;
    str[len] = 0;
  }
  return cnt;
}

unsigned int
StringUtils::chomp(std::string & str) {
  int cnt = 0;
  while (!str.empty() && (str[str.size() - 1] == '\n' || str[str.size() - 1] == '\r')) {
    str[str.size() - 1] = 0;
    cnt++;
  }
  return cnt;
}

static inline char to_hex_digit(int n) {
  if (n >= 0 && n <= 9) {
    return (char)('0' + n);
  } else if (n >= 10 && n <= 15) {
    return (char)('A' + n - 10);
  } else {
    return 0;
  }
}

string
StringUtils::hexencode(const unsigned char * data, size_t len) {
  string output;
  for (unsigned int i = 0; i < len; i++) {
    unsigned char c = data[i];
    output += to_hex_digit(c >> 4);
    output += to_hex_digit(c & 0x0f);
  }
  return output;
}

string
StringUtils::quote(const string & str) {
  string output = "\"";
  for (unsigned int i = 0; i < str.size(); i++) {
    if (str[i] == '"') output += "\\\"";
    else if (str[i] == '\\') output += "\\\\";
    else if (str[i] == 0x0a) output += "\\n";
    else if (str[i] == 0x0d) { /** skip **/ }
    else output += str[i];
  }
  output += "\"";
  return output;
}

bool
StringUtils::isNumber(const char * s) {
  if (!*s) return false;
  unsigned int i = 0;
  if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-') {
    return false;
  }
  for (i++; s[i] != 0; i++) {
    if (!isdigit(s[i])) {
      return false;
    }
  }
  return true;
}

static inline uint32_t to_lower_unicode(uint32_t c) {
  if ((c >= 'A' && c <= 'Z') ||
      (c >= 192 && c <= 214) || // Agrave to Ouml
      (c >= 216 && c <= 222) || // Oslash to Thorn
      (c >= 0x0410 && c <= 0x042f) // Cyrillic capitals
      ) {
    return c + 0x20;
  } else {
    return c;
  }
}

static inline uint32_t to_upper_unicode(uint32_t c) {
  if ((c >= 'a' && c <= 'z') ||
      (c >= 224 && c <= 246) || // Agrave to Ouml
      (c >= 248 && c <= 254) || // Oslash to Thorn
      (c >= 0x0430 && c <= 0x044F) // Cyrillic capitals
      ) {
    return c - 0x20;
  } else {
    return c;
  }
}

size_t
StringUtils::length(const std::string & input) {
  size_t r = 0;
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 

  while ( str_i < end ) {
    utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    r++;
  }
  return r;  
}

string
StringUtils::reverse(const string & input) {
  auto v = toVector(input);
  string r;
  auto r_inserter = back_inserter(r);
  for (size_t i = 0; i < v.size(); i++) {
    utf8::append(v[v.size() - 1 - i], r_inserter);   
  }

  return r;  
}

bool
StringUtils::isValidUTF8(const std::string & input) {
  return utf8::is_valid(input.c_str(), input.c_str() + input.size());    
}

string
StringUtils::toLower(const string & input) {
  string r;
  r.reserve(input.size());

  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 

  auto output_inserter = back_inserter(r);

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    utf8::append(to_lower_unicode(c), output_inserter);
  }
  return r;
}

string
StringUtils::toUpper(const string & input) {
  string r;
  r.reserve(input.size());

  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 

  auto output_inserter = back_inserter(r);

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    utf8::append(to_upper_unicode(c), output_inserter);
  }
  return r;
}

bool
StringUtils::isUpper(const string & input) {
  if (input.empty()) {
    return false;
  } else {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size(); 
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
      if (!is_upper_unicode(c)) {
	return false;
      }
    }
    return true;
  }
}

bool
StringUtils::isBlank(const std::string & input) {
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (!is_space_unicode(c)) {
      return false;
    }
  }
  return true;
}

string
StringUtils::extractAscii(const std::string & s) {
  string r;
  r.reserve(s.size());
  for (unsigned int i = 0; i < s.size(); i++) {
    if (s[i] >= 0) r += s[i];
  }
  return r;
}

string
StringUtils::extractNumber(const std::string & s) {
  string r;
  r.reserve(s.size());
  for (unsigned int i = 0; i < s.size(); i++) {
    if (isdigit(s[i])) r += s[i];
  }
  return r;
}

string
StringUtils::decodeEntities(const string & s) {
  string r;
  for (unsigned int i = 0; i < s.size(); ) {
    if (s[i] == '&') {
      if (i + 4 <= s.size() && s[i + 1] == 'l' && s[i + 2] == 't' && s[i + 3] == ';') {
	r += '<';
	i += 4;
	continue;
      } else if (i + 4 <= s.size() && s[i + 1] == 'g' && s[i + 2] == 't' && s[i + 3] == ';') {
	r += '>';
	i += 4;
	continue;
      } else if (i + 5 <= s.size() && s[i + 1] == 'a' && s[i + 2] == 'm' && s[i + 3] == 'p' && s[i + 4] == ';') {
	r += '&';
	i += 5;
	continue;
      } else if (i + 6 <= s.size() && s[i + 1] == 'q' && s[i + 2] == 'u' && s[i + 3] == 'o' && s[i + 4] == 't' && s[i + 5] == ';') {
	r += '"';
	i += 6;
	continue;
      } else if (i + 6 <= s.size() && s[i + 1] == 'a' && s[i + 2] == 'p' && s[i + 3] == 'o' && s[i + 4] == 's' && s[i + 5] == ';') {
	r += '\'';
	i += 6;
	continue;
      }
    }
    r += s[i];
    i++;
  }
  return r;
}

#define PREFIX "xn--"
#define BASE 36
#define TMIN 1
#define TMAX 26
#define SKEW 38
#define DAMP 700
#define INITIAL_BIAS 72
#define INITIAL_N 128
#define DELIMITER 0x2d
// my $BasicRE   = qr/[\x00-\x7f]/;

using namespace std;

static int digit_value(uint32_t code) {
  if (code >= 'A' && code <= 'Z') {
    return code - 'A';
  } else if (code >= 'a' && code <= 'z') {
    return code - 'a';
  } else if (code >= '0' && code <= '9') {
    return code - '0' + 26;
  } else {
    assert(0);
    return -1;
  }
}

static uint32_t code_point(int digit) {
  if (0 <= digit && digit <= 25) {
    return 'a' + digit;
  } else if (26 <= digit && digit <= 36) {
    return '0' + digit - 26;
  } else {
    assert(0);
    return '?';
  }
}

static int adapt(int delta, int numpoints, bool firsttime) {
  delta = firsttime ? delta / DAMP : delta / 2;
  delta += delta / numpoints;
  int k = 0;
  while (delta > ((BASE - TMIN) * TMAX) / 2) {
    delta /= BASE - TMIN;
    k += BASE;
  }
  return k + (((BASE - TMIN + 1) * delta) / (delta + SKEW));
}

string
StringUtils::encodePunycode(const string & input) {
  int n = INITIAL_N;
  int delta = 0;
  int bias = INITIAL_BIAS;

  string output, basic;

  for (unsigned int i = 0; i < input.size(); i++) {
    if (input[i] >= 0) basic += input[i];
  }

  int h = basic.size(), b = (int)basic.size();
  if (b > 0) output += basic;

  // cerr << "basic codepoints: " << output << endl;
  
  if (h < input.size()) {
    output = PREFIX + output;
    output += DELIMITER;
  } else {
    return output;
  }

  auto output_inserter = back_inserter(output);

  vector<uint32_t> input2;
  
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    input2.push_back(c);
  }
    
  while (h < input2.size()) {
    uint32_t m = 0;
    for (vector<uint32_t>::iterator it = input2.begin(); it != input2.end(); it++) {
      if (*it > n && (m == 0 || *it < m)) {
	m = *it;
      } 
    }
    // my $m = min(grep { $_ >= $n } map ord, @input);
    // cerr << "next code point to insert is " << m << endl;
    delta += (m - n) * (h + 1);
    n = m;
    for (vector<uint32_t>::iterator it = input2.begin(); it != input2.end(); it++) {
      uint32_t & c = *it;
      if (c < n) delta++;
      if (c == n) {
	  int q = delta;
	  for (int k = BASE; ; k += BASE) {
	    int t = (k <= bias) ? TMIN : (k >= bias + TMAX) ? TMAX : k - bias;
	    if (q < t) break;
	    uint32_t cp = code_point(t + ((q - t) % (BASE - t)));
	    utf8::append(cp, output_inserter);
	    q = (q - t) / (BASE - t);
	  }
	  utf8::append(code_point(q), output_inserter);
	  bias = adapt(delta, h + 1, h == b);
	  // cerr << "bias becomes " << bias << endl;
	  delta = 0;
	  h++;
      }
    }
    delta++;
    n++;
  }

  return output;
}

string
StringUtils::removeRepeatingLetters(const std::string & input) {
  unsigned int current_repeat = 0;
  uint32_t prev_c = 0, prev_lcc = 0;
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();
  string output;
  auto output_inserter = back_inserter(output);
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    uint32_t lcc = tolower(c);
    if (is_alpha_unicode(lcc) && lcc == prev_lcc) {
      current_repeat++;
    } else {
      if (prev_c) {
	if (current_repeat > 2) current_repeat = 2;
	for (unsigned int i = 0; i < current_repeat; i++) {
	  utf8::append(prev_c, output_inserter);
	}
      } 
      current_repeat = 1;
      prev_c = c;
      prev_lcc = lcc;
    }
  }
  if (current_repeat > 2) current_repeat = 2;
  for (unsigned int i = 0; i < current_repeat; i++) {
    utf8::append(prev_c, output_inserter);
  }
  return output;
}

vector<uint32_t>
StringUtils::toVector(const std::string & input) {
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();
  
  vector<uint32_t> r;
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    r.push_back(c);
  }
  return r;
}

string
StringUtils::fromVector(const vector<uint32_t> & input) {
  string r;
  auto output_inserter = back_inserter(r);
  for (auto c : input) {
    utf8::append(c, output_inserter);
  }
  return r;
}

// sub decode_punycode {
//   my $code = shift;
  
//   my $n	  = INITIAL_N;
//   my $i	  = 0;
//   my $bias   = INITIAL_BIAS;
//   my @output;
  
//   if ($PREFIX) {
//     if ($code !~ /^$PREFIX/) {
//       return $code;
//     }
//     $code =~ s/^$PREFIX//;
//   }
  
//   if ($code =~ s/(.*)$Delimiter//o) {
//     push @output, map ord, split //, $1;
//     return _croak('non-basic code point') unless $1 =~ /^$BasicRE*$/o;
//   }
  
//   while ($code) {
//     my $oldi = $i;
//     my $w	= 1;
//   LOOP:
//     for (my $k = BASE; 1; $k += BASE) {
//       my $cp = substr($code, 0, 1, '');
//       my $digit = digit_value($cp);
//       defined $digit or return _croak("invalid punycode input");
//       $i += $digit * $w;
//       my $t = ($k <= $bias) ? TMIN
// 	: ($k >= $bias + TMAX) ? TMAX : $k - $bias;
//       last LOOP if $digit < $t;
//       $w *= (BASE - $t);
//     }
//     $bias = adapt($i - $oldi, @output + 1, $oldi == 0);
//     warn "bias becomes $bias" if $DEBUG;
//     $n += $i / (@output + 1);
//     $i = $i % (@output + 1);
//     splice(@output, $i, 0, $n);
//     warn join " ", map sprintf('%04x', $_), @output if $DEBUG;
//     $i++;
//   }
//   return join '', map chr, @output;
// }


string
StringUtils::createNonce(unsigned int size) {
  string nonce;
  for (unsigned int i = 0; i < size; i++) {
    int v = rand() % 62;
    if (v < 26) {
      nonce += char('A' + v);
    } else if (v < 52) {
      nonce += char('a' + v - 26);
    } else {
      nonce += char('0' + v - 52);
    }
  }
  return nonce;
}

std::string
StringUtils::substr(const std::string & input, size_t start, size_t l) {
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();

  string output;
  auto output_inserter = back_inserter(output);
  for (unsigned int i = 0; i < start + l && str_i < end; i++) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (i >= start) {
      utf8::append(c, output_inserter);
    }
  }
  
  return output;
}

bool
StringUtils::isWord(const string & input) {
  if (input.empty()) {
    return false;
  } else {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size(); 
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
      if (!is_alpha_unicode(c) && !is_syllabary_unicode(c) && !is_ideograph_unicode(c)) {
	return false;
      }
    }
    return true;
  }
}

bool
StringUtils::hasWordCharacter(const string & input) {
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (is_alpha_unicode(c) || is_syllabary_unicode(c) || is_ideograph_unicode(c)) {
      return true;
    }
  }
  return false;
}

void
StringUtils::repairXML(std::string & content) {
  for (size_t index = 0; ; ) {                                                                                                                                                                          
    index = content.find("<video loop ", index);                                                                                                                                                        
    if (index == string::npos) break;                                                                                                                                                                   
    
    content.replace(index, 12, "<video loop=\"\" ");                                                                                                                                                    
    index += 15;                                                                                                                                                                                        
  }                                                                                                                                                                                                     
  
  for (size_t index = 0; ; ) {                                                                                                                                                                          
    index = content.find("<img ", index);                                                                                                                                                               
    if (index == string::npos) break;                                                                                                                                                                   
    index = content.find('>', index);                                                                                                                                                                   
    if (index == string::npos) break;                                                                                                                                                                   

    if (content[index - 1] != '/') {
      content.replace(index, 1, "/>");                                                                                                                                                                    
      index += 2;
    }
  }

  for (size_t index = 0; ; ) {
    index = content.find("<script", index);
    if (index == string::npos) break;
    auto index2 = content.find("</script>", index + 7);
    if (index2 == string::npos) break;
    content.erase(index, index2 - index + 9);
  }

  for (size_t index = 0; ; ) {
    index = content.find("<link", index);
    if (index == string::npos) break;
    auto index2 = content.find('>', index + 5);
    if (index2 == string::npos) break;
    content.erase(index, index2 - index + 1);
  }

  for (size_t index = 0; ; ) {
    index = content.find("<meta", index);
    if (index == string::npos) break;
    auto index2 = content.find('>', index + 5);
    if (index2 == string::npos) break;
    content.erase(index, index2 - index + 1);
  }
}

  // #define START_BOUNDARY	"(?<=[\\s,.:;\"']|^)"
#define WORD_CHARACTER	"[^\\s,.:;\"'!?\\(\\)\\[\\]{}/\\\\#%&=+^$*<>|@~]"
#define START_BOUNDARY	"(?:[\\s,.:;\"'!?\\(\\)\\[\\]{}/\\\\#%&=+^$*<>|@~-]|^)"
#define END_BOUNDARY	"(?=[\\s,.:;\"'!?\\(\\)\\[\\]{}/\\\\#%&=+^$*<>|@~-]|$)"
  
std::string
StringUtils::createRegex(std::string keyword) {
  bool starts_with_wildcard = false, ends_with_wildcard = false;
  
  if (keyword[0] == '*') {
    starts_with_wildcard = true;
    keyword.erase(0, 1);
  }
  if (!keyword.empty() && keyword[keyword.size() - 1] == '*') {
    ends_with_wildcard = true;
    keyword.erase(keyword.size() - 1);
  }
  
  std::string r;
  
  if (!starts_with_wildcard && keyword[0] != '#' && keyword[0] != '@') r += START_BOUNDARY;

  const char * str = keyword.c_str();
  const char * str_i = str;
  const char * end = str + keyword.size(); 

  auto output_inserter = back_inserter(r);

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (c == '?') {
      r += WORD_CHARACTER;
    } else if (c == '.' || c == '*' || c == '(' || c == ')' || c == '[' || c == ']' || c == '|') {
      utf8::append('\\', output_inserter);
      utf8::append(c, output_inserter);
    } else {
      utf8::append(to_lower_unicode(c), output_inserter);
    }
  }
  
  if (!ends_with_wildcard) r += END_BOUNDARY;
  return r;
}

std::string
StringUtils::normalizeNFC(const std::string & input) {
  auto r0 = utf8proc_NFC((const unsigned char *)input.c_str());
  string r;
  if (r0) {
    r = (const char *)r0;
    free(r0);
  }
  return r;
}
