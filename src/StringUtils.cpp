#include <StringUtils.h>

#include <cstring>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <cassert>

#include <character_tests.h>

#include "utf8/utf8.h"

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
  
  string tmp;
  string part;
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (is_alpha_unicode(c)) {
      tmp += part;
      part.clear();
      utf8::append(c, back_inserter(tmp));
    } else {
      utf8::append(c, back_inserter(part));
    }
  }
  
  input = tmp;
#endif
}

string
StringUtils::normalizeText(const string & input) {
  string r;
  bool is_whitespace = false;
  
  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();

  try {
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end);
      if (!is_space_unicode(c)) {
	if (is_whitespace) {
	  r += ' ';
	  is_whitespace = false;
	}
	utf8::append(c, back_inserter(r));
      } else if (!is_whitespace && !r.empty()) {
	is_whitespace = true;
      }
    }
  } catch (utf8::invalid_utf8 & e) {
    cerr << "StringUtils: invalid utf8: " << input << "\n";
  }
  return r;
}

void
StringUtils::trimUtf8(string & s) {
  string n;
  int state = 1;
  string output, waiting_whitespace;

  const char * str = s.c_str();
  const char * str_i = str;
  const char * end = str + s.size();
  
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    assert(c);
    
    bool is_whitespace = is_space_unicode(c);

    switch (state) {
    case 1:
      if (!is_whitespace) {
	state++;
	utf8::append(c, back_inserter(output));
      }
      break;
    case 2:
      if (is_whitespace) {
	utf8::append(c, back_inserter(waiting_whitespace));
      } else {
	output += waiting_whitespace;
	waiting_whitespace.clear();
	utf8::append(c, back_inserter(output));
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

// empty string returns empty vector
// otherwise returns delimiter count + 1 strings
vector<string>
StringUtils::split(const string & line, char delimiter) {
  vector<string> v;
  // back_insert_iterator< vector<string> > backiter(v);
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

string
StringUtils::toLower(const string & input) {
  string r;
  r.reserve(input.size());

  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size(); 

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    utf8::append(to_lower_unicode(c), back_inserter(r));
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

  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    utf8::append(to_upper_unicode(c), back_inserter(r));
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
      } else if (s[i + 1] == 'a' && s[i + 2] == 'm' && s[i + 3] == 'p' && s[i + 4] == ';') {
	r += '&';
	i += 5;
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

  int h = basic.size(), b = basic.size();
  if (b > 0) output += basic;

  cerr << "basic codepoints: " << output << endl;
  
  if (h < input.size()) {
    output = PREFIX + output;
    output += DELIMITER;
  } else {
    return output;
  }

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
    cerr << "next code point to insert is " << m << endl;
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
	    utf8::append(cp, back_inserter(output));
	    q = (q - t) / (BASE - t);
	  }
	  utf8::append(code_point(q), back_inserter(output));
	  bias = adapt(delta, h + 1, h == b);
	  cerr << "bias becomes " << bias << endl;
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
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    uint32_t lcc = tolower(c);
    if (is_alpha_unicode(lcc) && lcc == prev_lcc) {
      current_repeat++;
    } else {
      if (prev_c) {
	if (current_repeat > 2) current_repeat = 2;
	for (unsigned int i = 0; i < current_repeat; i++) {
	  utf8::append(prev_c, back_inserter(output));
	}
      } 
      current_repeat = 1;
      prev_c = c;
      prev_lcc = lcc;
    }
  }
  if (current_repeat > 2) current_repeat = 2;
  for (unsigned int i = 0; i < current_repeat; i++) {
    utf8::append(prev_c, back_inserter(output));
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
  for (auto it = input.begin(); it != input.end(); it++) {
    utf8::append(*it, back_inserter(r));
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
  for (unsigned int i = 0; i < start + l && str_i < end; i++) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (i >= start) {
      utf8::append(c, back_inserter(output));
    }
  }
  
  return output;
}

bool
StringUtils::isAlpha(const string & input) {
  if (input.empty()) {
    return false;
  } else {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size(); 
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
      if (!is_alpha_unicode(c)) {
	return false;
      }
    }
    return true;
  }
}
