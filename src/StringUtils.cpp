#include <StringUtils.h>

#include <cstring>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <cassert>

#include <URI.h>
#include "utf8/utf8.h"

using namespace std;

const char * StringUtils::WHITESPACE = " \t\b\x0a\x0d";
const char * StringUtils::PUNCTUATION = ",.;?!-*:_/\\()[]{}";

bool StringUtils::initialized = false;
std::map<std::string, int> StringUtils::entities;

void
StringUtils::initialize() {
  initialized = true;
  
  entities["rsquor"] = 0x2019;
  entities["rsquor"] = 0x201a;
  entities["rdquor"] = 0x201d;
  entities["ldquor"] = 0x201e;

  entities["quot"] = entities["quo"] = 34;
  entities["amp"] = entities["AMP"] = 38;
  entities["apos"] = 39;
  entities["lt"] = 60;
  entities["gt"] = entities["GT"] = 62;
  entities["nbsp"] = 160;
  entities["NBSP"] = 160; // non-standard
  entities["iexcl"] = 161;
  entities["cent"] = 162;
  entities["pound"] = 163;
  entities["curren"] = 164;
  entities["yen"] = 165;
  entities["brvbar"] = 166;
  entities["sect"] = 167;
  entities["uml"] = 168;
  entities["copy"] = 169;
  entities["ordf"] = 170;
  entities["laquo"] = 171;
  entities["not"] = 172;
  entities["shy"] = 173;
  entities["reg"] = 174;
  entities["macr"] = 175;
  entities["deg"] = 176;
  entities["plusmn"] = 177;
  entities["sup2"] = 178;    
  entities["sup3"] = 179;
  entities["acute"] = 180;
  entities["micro"] = 181;
  entities["para"] = 182;
  entities["middot"] = 183;
  entities["cedil"] = 184;
  entities["sup1"] = 185;
  entities["ordm"] = 186;
  entities["raquo"] = 187;
  entities["frac14"] = 188;
  entities["frac12"] = 189;
  entities["frac34"] = 190;
  entities["iquest"] = 191;
  entities["Agrave"] = 192;
  entities["Aacute"] = 193;
  entities["Acirc"] = 194;
  entities["Atilde"] = 195;
  entities["Auml"] = 196;
  entities["Aring"] = 197;
  entities["AElig"] = 198;
  entities["Ccedil"] = 199;
  entities["Egrave"] = 200;
  entities["Eacute"] = 201;
  entities["Ecirc"] = 202;
  entities["Euml"] = 203;
  entities["Igrave"] = 204;
  entities["Iacute"] = 205;
  entities["Icirc"] = 206;
  entities["Iuml"] = 207;
  entities["ETH"] = 208;
  entities["Ntilde"] = 209;
  entities["Ograve"] = 210;
  entities["Oacute"] = 211;
  entities["Ocirc"] = 212;
  entities["Otilde"] = 213;
  entities["Ouml"] = 214;
  entities["times"] = 215;
  entities["Oslash"] = 216;
  entities["Ugrave"] = 217;
  entities["Uacute"] = 218;
  entities["Ucirc"] = 219;
  entities["Uuml"] = 220;   
  entities["Yacute"] = 221;
  entities["THORN"] = 222;
  entities["szlig"] = 223;
  entities["agrave"] = 224;
  entities["aacute"] = 225;
  entities["acirc"] = 226;
  entities["atilde"] = 227;
  entities["auml"] = 228;
  entities["aring"] = 229;
  entities["aelig"] = 230;
  entities["ccedil"] = 231;
  entities["egrave"] = 232;
  entities["eacute"] = 233;
  entities["ecirc"] = 234;
  entities["euml"] = 235;
  entities["igrave"] = 236;
  entities["iacute"] = 237;
  entities["icirc"] = 238;
  entities["iuml"] = 239;
  entities["eth"] = 240;
  entities["ntilde"] = 241;
  entities["ograve"] = 242;
  entities["oacute"] = 243;
  entities["ocirc"] = 244;
  entities["otilde"] = 245;
  entities["ouml"] = 246;
  entities["divide"] = 247;
  entities["oslash"] = 248;
  entities["ugrave"] = 249;
  entities["uacute"] = 250;
  entities["ucirc"] = 251;
  entities["uuml"] = 252;
  entities["yacute"] = 253;
  entities["thorn"] = 254;
  entities["yuml"] = 255;
  entities["OElig"] = 338;
  entities["oelig"] = 339;
  entities["Scaron"] = 352;
  entities["scaron"] = 353;
  entities["Yuml"] = 376;
  entities["fnof"] = 402;
  entities["Alpha"] = 913;
  entities["Beta"] = 914;
  entities["Gamma"] = 915;
  entities["Delta"] = 916;
  entities["Epsilon"] = 917;
  entities["Zeta"] = 918;
  entities["Eta"] = 919;
  entities["Theta"] = 920;
  entities["Iota"] = 921;
  entities["Kappa"] = 922;
  entities["Lambda"] = 923;
  entities["Mu"] = 924;
  entities["Nu"] = 925;
  entities["Xi"] = 926;
  entities["Omicron"] = 927;
  entities["Pi"] = 928;
  entities["Rho"] = 929;
  // no Sigmaf
  entities["Sigma"] = 931;
  entities["Tau"] = 932;
  entities["Upsilon"] = 933;
  entities["Phi"] = 934;
  entities["Chi"] = 935;
  entities["Psi"] = 936;
  entities["Omega"] = 937;
  entities["alpha"] = 945;
  entities["beta"] = 946;
  entities["gamma"] = 947;
  entities["delta"] = 948;
  entities["epsilon"] = 949;
  entities["zeta"] = 950;
  entities["eta"] = 951;
  entities["theta"] = 952;
  entities["iota"] = 953;
  entities["kappa"] = 954;
  entities["lambda"] = 955;
  entities["mu"] = 956;
  entities["nu"] = 957;  
  entities["xi"] = 958;
  entities["omicron"] = 959;
  entities["pi"] = 960;
  entities["rho"] = 961;
  entities["sigmaf"] = 962;
  entities["sigma"] = 963;
  entities["tau"] = 964;
  entities["upsilon"] = 965;
  entities["phi"] = 966;
  entities["chi"] = 967;
  entities["psi"] = 968;
  entities["omega"] = 969;
  entities["ensp"] = 8194; // em space
  entities["emsp"] = 8195; // em space
  entities["thinsp"] = 8201;
  entities["lrm"] = 8206; // LEFT-TO-RIGHT MARK
  entities["rlm"] = 8207; // RIGHT-TO-LEFT MARK
  entities["dash"] = 8208;
  entities["ndash"] = 8211;
  entities["mdash"] = 8212;
  entities["lsquo"] = 8216;
  entities["rsquo"] = 8217;
  entities["sbquo"] = 8218;
  entities["ldquo"] = 8220;
  entities["rdquo"] = 8221;
  entities["bdquo"] = 8222;
  entities["dagger"] = 8224;
  entities["Dagger"] = 8225;
  entities["bull"] = entities["bullet"] = 8226; // bullet is not valid
  entities["hellip"] = entities["helip"] = 8230; // helip is typo
  entities["prime"] = 8242;
  entities["Prime"] = 8243;
  entities["lsaquo"] = 8249;
  entities["rsaquo"] = 8250;
  entities["oline"] = 8254;
  entities["frasl"] = 8260;
  entities["euro"] = 8364;
  entities["image"] = 8465;
  entities["weierp"] = 8472;
  entities["real"] = 8476;
  entities["trade"] = 8482;
  entities["alefsym"] = 8501;
  entities["larr"] = 8592;
  entities["uarr"] = 8593;
  entities["rarr"] = 8594;
  entities["darr"] = 8595;
  entities["harr"] = 8596;
  entities["map"] = 8614;
  entities["crarr"] = 8629;
  entities["lArr"] = 8656;
  entities["uArr"] = 8657;
  entities["rArr"] = 8658;
  entities["dArr"] = 8659;
  entities["hArr"] = 8660;
  entities["forall"] = 8704;
  entities["part"] = 8706;
  entities["exist"] = 8707;
  entities["empty"] = 8709;
  entities["nabla"] = 8711;
  entities["isin"] = 8712;
  entities["notin"] = 8713;
  entities["minus"] = 8722;
  entities["radic"] = 8730;
  entities["infin"] = 8734;
  entities["or"] = 8744;
  entities["cap"] = 8745;
  entities["sim"] = 8764;
  entities["cong"] = 8773;
  entities["asymp"] = 8776;
  entities["ne"] = 8800;
  entities["equiv"] = 8801;
  entities["le"] = 8804;
  entities["ge"] = 8805;
  entities["sub"] = 8834;
  entities["sup"] = 8835;
  entities["nsub"] = 8836;
  entities["supe"] = 8839;
  entities["oplus"] = 8853;
  entities["otimes"] = 8855;
  entities["perp"] = 8869;
  entities["sdot"] = 8901;
  entities["loz"] = 9674;
  entities["spades"] = 9824;
  entities["clubs"] = 9827;
  entities["hearts"] = 9829;
  entities["diams"] = 9830;
  // entities["actionquot"] = 
}

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
    if (isAlpha(c)) {
      utf8::prior(str_i, str);
      break;
    }
  }
  
  string tmp;
  string part;
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    if (isAlpha(c)) {
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

bool
StringUtils::isBlank(uint32_t c ) {
  switch (c) {
  case ' ': return true; // space
  case '\t': return true; // tab
  case 8194: return true; // en space
  case 8195: return true; // em space
  case 8201: return true; // THIN SPACE
  case 160: return true; // nbsp
  default: return false;
  }
}

bool
StringUtils::isSpace(uint32_t cp) { 
  return isBlank(cp) ||
    cp == 173 || // soft hyphen
    cp == '\b' ||
    cp == 0x0a ||
    cp == 0x0d;
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
    
    bool is_whitespace = c == '\t' || c == '\b' || c == '\x0a' || c == '\x0d' || isBlank(c);

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

static int get_xdigit(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } else {
    return 0;
  }
}

static char to_hex_digit(int n) {
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
StringUtils::isDigit(uint32_t c) {
  return c >= '0' && c <= '9';
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

bool
StringUtils::isBlank(const char * s) {
  for (unsigned int i = 0; s[i] != 0; i++) {
    if (!isspace(s[i])) {
      return false;
    }
  }
  return true;
}

uint32_t
StringUtils::toLower(uint32_t c) {
  if (c <= 127) {
    return (char)tolower((char)c);
  } else if ((c >= 192 && c <= 214) || // Agrave to Ouml
	     (c >= 216 && c <= 222) // Oslash to Thorn
	     ) {
    return c + 32;
  } else if (c >= 0x0410 && c <= 0x042f) { // Cyrillic capitals
    return c + 0x20;
  } else {
    return c;
  }
}

uint32_t
StringUtils::toUpper(uint32_t c) {
  if (c <= 127) {
    return (char)toupper((char)c);
  } else if ((c >= 224 && c <= 246) || // Agrave to Ouml
	     (c >= 248 && c <= 254) // Oslash to Thorn
	     ) {
    return c - 32;
  } else if (c >= 0x0430 && c <= 0x044F) { // Cyrillic capitals
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
    utf8::append(toLower(c), back_inserter(r));
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
    utf8::append(toUpper(c), back_inserter(r));
  }
  return r;
}

bool
StringUtils::isLower(uint32_t c) {
  if (c <= 127) {
    return islower((char)c);
  } else if ((c >= 192 + 32 && c <= 214 + 32) || // Agrave to Ouml
	     (c >= 216 + 32 && c <= 221 + 32) // Oslash to Yacute
	     ) {
    return true;
  } else if (c >= 0x0410 + 32 && c <= 0x042f + 32) { // Cyrillic capitals
    return true;
  } else {
    return false;
  }
}

bool
StringUtils::isUpper(uint32_t c) {
  if (c <= 127) {
    return isupper((char)c);
  } else if ((c >= 192 && c <= 214) || // Agrave to Ouml
	     (c >= 216 && c <= 221) // Oslash to Yacute
	     ) {
    return true;
  } else if (c >= 0x0410 && c <= 0x042f) { // Cyrillic capitals
    return true;
  } else {
    return false;
  }
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
      if (!isUpper(c)) {
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

bool
StringUtils::convertEntity(const std::string & entity, std::string & r) {
  if (!initialized) {
    initialize();
  }
  
  int value = 0;
  if (!entity.empty()) {
    if (entity[0] == '#') {
      if (!entity.empty() && (entity[1] == 'x' || entity[1] == 'X')) {
	for (int i = 2; i < entity.size(); i++) {
	  value = 16 * value + get_xdigit(entity[i]);
	}
      } else {
	string part = entity.substr(1);
	assert(isNumber(part));
	value = atoi(part.c_str());
      }
    } else {
      map<string, int>::const_iterator it = entities.find(entity);
      if (it != entities.end()) {
	value = it->second;
      } else {
	cerr << "entity " << entity << " not found.\n";
	return false;
      }
    }
  }

  if (!value) {
    return false;
  } else if (isBlank(value)) {
    r += " ";
    return true;
  } else if (value == 173) { // soft hyphen
    // nothing
    return true;
  } else {
    string tmp;
    utf8::append(value, back_inserter(tmp));
    assert(tmp.size());
    r += tmp;
    return true;
  }
}

#define STATE_INITIAL			1
#define STATE_WHITESPACE		2
#define STATE_ENTITY			3
#define STATE_TAG_START			4
#define STATE_TAG			5
#define STATE_TAG_ATTRIBUTE_START	6
#define STATE_TAG_ATTRIBUTE_WHITESPACE	7
#define STATE_TAG_ATTRIBUTE_QUOTE	8
#define STATE_TAG_ATTRIBUTE_COMMA	9
#define STATE_TAG_ATTRIBUTE_BARE	10
#define STATE_COMMENT_S1		11 // <!
#define STATE_COMMENT_S2		12 // <!-
#define STATE_COMMENT			13
#define STATE_COMMENT_E1		14
#define STATE_COMMENT_E2		15
#define STATE_SCRIPT			16 // </script>
#define STATE_SCRIPT_E1			17 // /script>
#define STATE_SCRIPT_E2			18 // script>
#define STATE_SCRIPT_E3			19 // cript>
#define STATE_SCRIPT_E4			20 // ript>
#define STATE_SCRIPT_E5			21 // ipt>
#define STATE_SCRIPT_E6			22 // pt>
#define STATE_SCRIPT_E7			23 // t>
#define STATE_SCRIPT_E8			24 // >
#define STATE_STYLE			25 // </style>
#define STATE_STYLE_E1		        26 // /style>
#define STATE_STYLE_E2			27 // style>
#define STATE_STYLE_E3			28 // tyle>
#define STATE_STYLE_E4			29 // yle>
#define STATE_STYLE_E5			30 // le>
#define STATE_STYLE_E6			31 // e>
#define STATE_STYLE_E7			32 // >


// #define 
// #define STATE_SCRIPT		7
// #define STATE_SCRIPT_1

string
StringUtils::stripHTML(const string & input, bool strip_html) {
  int state = STATE_WHITESPACE;
  string r;
  string current_token;
  for (unsigned int i = 0; i < input.size(); i++) {
    char c = input[i];
    if (c == '\r') continue;
    switch (state) {
    case STATE_INITIAL:
    case STATE_WHITESPACE:
      if (c == '&') {
	assert(current_token.empty());
	state = STATE_ENTITY;
      } else if (c == '<' && strip_html) {
	state = STATE_TAG_START;
      } else if (isspace(c)) {
	if (state == STATE_INITIAL) {
	  state = STATE_WHITESPACE;
	  r += ' ';
	}
      } else {
	state = STATE_INITIAL;
	r += c;
      }
      break;
    case STATE_ENTITY:
      if (c == ';') {
	const string & entity = current_token;
	if (!convertEntity(entity, r)) {
	  cerr << "invalid entity " << entity << endl;
	  // linkAssert(entity == "serviceBg" || entity == "nbs" || entity == "M" || entity == "ampamp" || entity == "aamp" || entity == "sec" || entity == "A" || entity == "cacute"); // sallitut virheet
	  r += '&';
	  r += entity;
	  r += ';';
	}
	current_token.clear();
	state = STATE_INITIAL;
      } else if (!isAlpha(c) && c != '#' && !isDigit(c)) { // unterminated entity
	r += '&';
	r += current_token;
	current_token.clear();
	state = STATE_INITIAL;
	continue; // what's this?
      } else {
	current_token += c;
      }
      break;
    case STATE_TAG_START:
      if (c == '3') {
	r += "<3"; // heart!
	state = STATE_INITIAL;
	break;
      } else if (c == '!') {
	state = STATE_COMMENT_S1;
	break;
      }
      // pass
    case STATE_TAG:
      if (c == '>' || c == '<') {
	string tag = toLower(current_token);
	string::size_type pos = tag.find_first_of("/ \t\b\x0a");
	if (pos != string::npos) tag.erase(pos);
	if (tag == "br" || tag == "hr") {
	  r += '\n';
	  state = STATE_WHITESPACE;
	} else if (tag == "script") {
	  state = STATE_SCRIPT;
	} else if (tag == "style") {
	  state = STATE_STYLE;
	} else {
	  state = STATE_INITIAL;
	}
	current_token.clear();
	if (c == '<') {
	  cerr << "unclosed tag\n";
	  i--;
	  break;
	}
      } else if (c == '=') {
	current_token += c;
	state = STATE_TAG_ATTRIBUTE_START;
      } else if (c == '"' || c == '\'') {
	cerr << "lone quote in tag\n";
	current_token += c;
      } else {
	current_token += c;
	state = STATE_TAG;
      }
      break;
    case STATE_TAG_ATTRIBUTE_START:
      if (isspace(c)) {
	state = STATE_TAG_ATTRIBUTE_WHITESPACE;
      	current_token += c;
      } else if (c == '"') {
	current_token += c;
	state = STATE_TAG_ATTRIBUTE_QUOTE;
      } else if (c == '\'') {
	current_token += c;
	state = STATE_TAG_ATTRIBUTE_COMMA;
      } else if (c == '>' || c == '<') {
	state = STATE_TAG;
	i--;
      } else {
	current_token += c;
	state = STATE_TAG_ATTRIBUTE_BARE;
      }
      break;
    case STATE_TAG_ATTRIBUTE_WHITESPACE:
      if (isspace(c)) {
	current_token += c;
      } else if (c == '"') {
	current_token += c;
	state = STATE_TAG_ATTRIBUTE_QUOTE;
      } else if (c == '\'') {
	current_token += c;
	state = STATE_TAG_ATTRIBUTE_COMMA;
      } else {
	state = STATE_TAG;
	i--;
      }
      break;
    case STATE_TAG_ATTRIBUTE_QUOTE:
      current_token += c;
      if (c == '"') state = STATE_TAG;
      break;
    case STATE_TAG_ATTRIBUTE_COMMA:
      current_token += c;
      if (c == '\'') state = STATE_TAG;
      break;
    case STATE_TAG_ATTRIBUTE_BARE:
      if (c == '>' || c == '<') {
	state = STATE_TAG;
	i--; // redo
      } else if (isspace(c) || c == '"' || c == '\'') {
	current_token += c;
	state = STATE_TAG;
      } else {
	current_token += c;
      }
      break;    
    case STATE_COMMENT_S1:
      if (c == '-') state = STATE_COMMENT_S2;
      else if (c == 'D') { // doctype
	current_token = "!D";
	state = STATE_TAG;
      } else {
	// tag with exclamation mark <!a
	current_token = "!";
	current_token += c;
	state = STATE_TAG;
      }
      break;
    case STATE_COMMENT_S2:
      if (c == '-') state = STATE_COMMENT;
      else {
	cerr << "invalid comment(1): " << input << endl;
	r += "<!-";
	r += c;
	state = STATE_INITIAL;
      }
      break;
    case STATE_COMMENT:
      if (c == '-') state = STATE_COMMENT_E1;
      break;
    case STATE_COMMENT_E1:
      if (c == '-') state = STATE_COMMENT_E2;
      else state = STATE_COMMENT;
      break;
    case STATE_COMMENT_E2:
      if (c == '>') state = STATE_INITIAL;
      else if (c != '-') state = STATE_COMMENT;
      break;
    case STATE_SCRIPT:
      if (c == '<') state = STATE_SCRIPT_E1;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E1:
      if (c == '/') state = STATE_SCRIPT_E2;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E2:
      if (tolower(c) == 's') state = STATE_SCRIPT_E3;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E3:
      if (tolower(c) == 'c') state = STATE_SCRIPT_E4;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E4:
      if (tolower(c) == 'r') state = STATE_SCRIPT_E5;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E5:
      if (tolower(c) == 'i') state = STATE_SCRIPT_E6;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E6:
      if (tolower(c) == 'p') state = STATE_SCRIPT_E7;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E7:
      if (tolower(c) == 't') state = STATE_SCRIPT_E8;
      else state = STATE_SCRIPT;
      break;
    case STATE_SCRIPT_E8:
      if (c == '>') state = STATE_INITIAL;
      else state = STATE_SCRIPT;
      break;
    case STATE_STYLE:
      if (c == '<') state = STATE_STYLE_E1;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E1:
      if (c == '/') state = STATE_STYLE_E2;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E2:
      if (tolower(c) == 's') state = STATE_STYLE_E3;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E3:
      if (tolower(c) == 't') state = STATE_STYLE_E4;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E4:
      if (tolower(c) == 'y') state = STATE_STYLE_E5;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E5:
      if (tolower(c) == 'l') state = STATE_STYLE_E6;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E6:
      if (tolower(c) == 'e') state = STATE_STYLE_E7;
      else state = STATE_STYLE;
      break;
    case STATE_STYLE_E7:
      if (tolower(c) == '>') state = STATE_INITIAL;
      else state = STATE_STYLE;
      break;
    default:
      assert(0);
    }
  }
  if (state == STATE_ENTITY) {
    r += '&';
    r += current_token;
  } else if (state == STATE_TAG_START || state == STATE_TAG) {
    cerr << "malformed html (tag not closed): " << input << "\n";
    // assert(0);
    r += '<';
    r += current_token;
  } else if (state == STATE_TAG_ATTRIBUTE_START || state == STATE_TAG_ATTRIBUTE_QUOTE || state == STATE_TAG_ATTRIBUTE_COMMA || state == STATE_TAG_ATTRIBUTE_BARE) {
    cerr << "malformed html (attribute not closed): " << input << "\n";
    r += '<';
    r += current_token;
  } else if (state >= STATE_COMMENT_S1 && state <= STATE_COMMENT_E2) {
    cerr << "comment not closed\n";
    assert(0);
  } else if (state >= STATE_SCRIPT && state <= STATE_SCRIPT_E8) {
    cerr << "script not closed\n";
    // assert(0);
  } else if (state >= STATE_STYLE && state <= STATE_STYLE_E7) {
    cerr << "style not closed\n";
    assert(0);
  }
  trim(r);
  return r;
}

bool
StringUtils::isAlpha(uint32_t cp) {
  if ((cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z')) {
    return true;
  } else if (cp >= 0xc0 && cp <= 0xff) { // ISO-8859-1
    if (cp == 0xd7) return false; // ×
    else if (cp == 0xf7) return false; // ÷
    else return true;
  } else if (cp >= 0x05d0 && cp <= 0x05ea) { // Hebrew letters
    return true;
  } else if (cp >= 0x0400 && cp <= 0x04FF) { // Cyrillic (fix this) [why?]
    return true;
  } else if ((cp >= 0x0e01 && cp <= 0x0e2e) || // Thai consonants
	     (cp >= 0x0e30 && cp <= 0x0e3a) ||	// Thai vowels
	     (cp >= 0x0e40 && cp <= 0x0e44)	// Thai vowels
	     ) { 
    return true;
  } else if ((cp >= 0x0391 && cp <= 0x03a9) ||
	     (cp >= 0x03b1 && cp <= 0x03c9)) { // greek letters big and small
    return true;
  } else if (cp >= 0x0600 && cp <= 0x06FF) { // Arabic 
    return true;
  } else if (cp >= 0x0100 && cp <= 0x017F) { // Latin Extended-A
    return true;
  } else if (cp >= 0x0180 && cp <= 0x024F) { // Latin Extended-B
    return true;
  } else if (cp >= 0x0E00 && cp <= 0x0E7F) { // Thai
    return true;
  } else if (cp >= 0x1E00 && cp <= 0x1EFF) { // Latin Extended Additional
    return true;
  }
  return false;
}

bool
StringUtils::isIdeograph(uint32_t cp) {
  if ((cp >= 0x4E00 && cp < 0x62FF) || // CJK Unified Ideographs
      (cp >= 0x6300 && cp < 0x77FF) || // CJK Unified Ideographs
      (cp >= 0x7800 && cp < 0x8CFF) || // 
      (cp >= 0x8D00 && cp < 0x9FFF) //
) {
    return true;
  } else {
    return false;
  }
}

bool
StringUtils::isArabic(uint32_t cp) {
  if (cp >= 0xFE70 && cp <= 0xFEFF) { // Arabic Presentation Forms-B
    return true;
  } else {
    return false;
  }
}

bool
StringUtils::isSyllabary(uint32_t cp) {
  if ((cp >= 0x3040 && cp <= 0x309f) || // Hiragana
      (cp >= 0x30a0 && cp <= 0x30ff) || // Katakana
      (cp >= 0x31f0 && cp <= 0x31ff) || // Katakana phonetic extensions
      (cp >= 0xAC00 && cp <= 0xD7AF) // Hangul Syllables
      ) {
    return true;
  } else {
    return false;
  }
}

bool
StringUtils::isComplexSymbol(uint32_t cp) {
  if ((cp >= 0x02B00 && cp <= 0x02BFF) || // Miscellaneous Symbols and Arrows
      (cp >= 0x025A0 && cp <= 0x025FF) || // Geometric Shapes
      cp == 10084) { // LINEAR B IDEOGRAM B105F MARE
    return true;
  } else if (cp >= 0x2190 && cp <= 0x21FF) { // Arrows
    return true;
  } else if (cp >= 0x2500 && cp <= 0x257f) { // Box drawing
    return true;
  } else if (cp >= 0x2580 && cp <= 0x259F) { // Block Elements
    return true;
  } else if (cp >= 0x2600 && cp <= 0x26ff) { // Miscellaneous Symbols
    return true;      
  } else if (cp >= 0x2700 && cp <= 0x27BF) { // Dingbats
    return true;
  } else if (cp >= 0x1F600 && cp <= 0x1F64F) { // Emoticons
    return true;
  } else if (cp >= 0x1F300 && cp <= 0x1F5FF) { // Miscellaneous Symbols and Pictograms
    return true;
  } else if (cp >= 0x1F680 && cp <= 0x1F6FF) { // Transport And Map Symbols
    return true;
  } else {
    return false;
  }
}

bool
StringUtils::isSymbol(uint32_t cp) {
  if (cp == '$' || // dollar
      cp == 163 || // POUND SIGN
      cp == 174 || // REGISTERED SIGN
      cp == 215 || // MULTIPLICATION SIGN
      cp == 3647 || // THAI CURRENCY SYMBOL BAHT
      cp == 8364 || // EURO SIGN
      cp == 8467 || // letterlike symbol: script small l
      cp == 10068 || // White Question Mark Ornament 
      cp == 10071 || // Heavy Exclamation Mark Symbol      
      cp == 8419 // COMBINING ENCLOSING KEYCAP
      ) {
    return true;
  } else if (isComplexSymbol(cp)) {
    return true;
  } else if (cp >= 0x2000 && cp <= 0x206F) { // General punctuation
    return true;
  } else if (cp >= 0x3000 && cp <= 0x303F) { // CJK Symbols and Punctuation
    return true;
  } else if (cp >= 0xFF00 && cp <= 0xFFEF) { // Halfwidth and Fullwidth Forms Block 
    return true;
  } else {
    return false;
  }
}

bool
StringUtils::isCombining(uint32_t cp) {
  if (cp == 0x032b) { // COMBINING INVERTED DOUBLE ARCH BELOW
    return true;
  } else {
    return false;
  }
}
bool
StringUtils::isPrivate(uint32_t cp) {
  if (cp >= 0xE000 && cp <= 0xF8FF) {
    return true;
  } else {
    return false;
  }
}

#define STATE_URL_HTTP1		100
#define STATE_URL_HTTP2		101
#define STATE_URL_HTTP3		102
#define STATE_URL_HTTP4		103
#define STATE_URL_HTTP5		104
#define STATE_URL_FTP1		105
#define STATE_URL_FTP2		106
#define STATE_URL_FTP3		107
#define STATE_URL_C_HTTP1	108
#define STATE_URL_C_HTTP2	109
#define STATE_URL_C_HTTP3	110
#define STATE_URL_C_HTTP4	111
#define STATE_URL_C_HTTP5	112
#define STATE_URL_COLON		113
#define STATE_URL_SLASH		114
#define STATE_URL_BODY_START	115
#define STATE_URL_BODY		116
#define STATE_URL_BODY_IPV6	117
#define STATE_SMILEY1_START	118
#define STATE_SMILEY2_START	119
#define STATE_URL_AMP1		120
#define STATE_URL_AMP2		121
#define STATE_URL_AMP3		122
#define STATE_URL_AMP4		123
#define STATE_URL_ARG_START	124
#define STATE_URL_UNDERSCORE	125
#define STATE_URL_SQ_BRACKET	126
#define STATE_URL_CU_BRACKET	127
#define STATE_URL_PARENS	128

#define STATE_WORD		129

#define STATE_MENTION_START	140
#define STATE_MENTION_LATIN	141
#define STATE_MENTION_IDEOGRAPH	142

#define STATE_HASHTAG		150

set<string>
StringUtils::extractLinks(const std::string & input, bool allow_utf8, bool extract_mentions, bool extract_hashtags) {
  int state = STATE_INITIAL;
  vector<string> entities;
  string current_token;

  const char * str = input.c_str();
  const char * str_i = str;
  const char * end = str + input.size();
  
  while ( str_i < end ) {
    uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
    // assert(c);

    if (c == 0xFE0F) { // VARIATION SELECTOR-16
      continue;
    }
    // utf8::append(code, symbol); // initialize array `symbol`
    // while ( str_i < end );
    // utf8::iterator<const char*> it(input.c_str(), input.c_str(), input.c_str() + input.size());
  
    // for (unsigned int i = 0; i < input.size(); i++) {
    // char c = input[i];
    
    switch (state) {
    case STATE_INITIAL:
      if (c == 'h' || c == 'H') {
	state = STATE_URL_HTTP1;
      } else if (c == 'f' || c == 'F') {
	state = STATE_URL_FTP1;
      } else if (c == '@' && extract_mentions) {
	state = STATE_MENTION_START;
	current_token = "@";
      } else if (c == '#' && extract_hashtags) {
	state = STATE_HASHTAG;
	current_token = "#";
      } else if (isAlpha(c) || isIdeograph(c) || isSyllabary(c) || isDigit(c) || c == '_' || c == '-') {
	state = STATE_WORD;
      }
      break;
    case STATE_WORD:
      if (c == 'h' || c == 'H') {
	state = STATE_URL_HTTP1;
      } else if (c == 'f' || c == 'F') {
	state = STATE_URL_FTP1;
      } else if (isSpace(c)) {
	state = STATE_INITIAL;
      }
      break;
    case STATE_MENTION_START:
      if (isAlpha(c) || c == '_' || isDigit(c)) {
	utf8::append(c, back_inserter(current_token));
	state = STATE_MENTION_LATIN;
      } else if (isIdeograph(c) || isSyllabary(c)) {
	utf8::append(c, back_inserter(current_token));
	state = STATE_MENTION_IDEOGRAPH;
      } else if (c == '@') {
	// ignore
      } else if (isSymbol(c) || isSpace(c) || c == ')' || c == ':' || c == '.' || c == '+' || c == ',' || c == '%' || c == '/' || c == '\'' || c == '?' || c == '!' || c == '<' || c == '-' ||
		 c == '&' || c == '*' || c == '^' || c == '"' || c == ']' || c == ';' || c == '=') {
	state = STATE_INITIAL;
	current_token.clear();
      } else if (c == '#') {
	state = STATE_INITIAL;
	utf8::prior(str_i, str);
      } else if (c == 127464) { // REGIONAL INDICATOR SYMBOL LETTER C (don't know what to do with this)
	state = STATE_INITIAL;
	current_token.clear();	
      } else {
	cerr << "unable to start mention(" << c << "): " << input << endl;
#if 0
	assert(0);
#else
	return set<string>();
#endif
      }
      break;
    case STATE_MENTION_LATIN:
      if (isAlpha(c) || isDigit(c) || c == '_') {
	utf8::append(c, back_inserter(current_token));
      } else if (c == ')' || c == ']' || c == '}' || c == '.' || c == ':' || c == ';' || c == '!' || c == '?' || c == '\'' || c == '"' || c == ',' || c == '&' ||
		 c == '*' || c == '<' || c == '>' || c == '^' || c == '$' || c == '(' || c == '/' || c == '+' || c == '%' || c == '=' || c == '-' || c == '|' ||
		 c == 8212 || // Em Dash
		 isSpace(c) || isSymbol(c) || isPrivate(c) || isIdeograph(c) || isSyllabary(c) || isArabic(c)
		 ) {
	// cerr << "adding mention " << current_token << endl;
	entities.push_back(current_token);
	current_token.clear();
	if (isSpace(c)) {
	  state = STATE_INITIAL;
	} else {
	  state = STATE_WORD;
	}
      } else if (c == '#' || c == '@') {
	// cerr << "adding mention " << current_token << endl;
	entities.push_back(current_token);
	current_token.clear();
	utf8::prior(str_i, str);
	state = STATE_INITIAL;
      } else {
	cerr << "invalid mention(" << c << "): " << input << "\n";
#if 0
	assert(0);
#else
	return set<string>();
#endif	
      }
      break;
    case STATE_MENTION_IDEOGRAPH:
      cerr << "unable to start ideograph mention(" << c << "): " << input << endl;
#if 0
      assert(0);
#else
      return set<string>();
#endif
      if (isSymbol(c)) {
	// finish
      }
      break;
    case STATE_HASHTAG:
      if (isAlpha(c) || isIdeograph(c) || isSyllabary(c) || isDigit(c) || c == '_' || c == '^' || c == '%' || c == '$' || c == '&' || c == '+' ||
	  c == 8364 || // euro
	  c == 215 || // multiplication sign
	  isCombining(c)
	  ) {
	utf8::append(c, back_inserter(current_token));
      } else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '.' || c == ':' || c == ';' || c == '!' || c == '?' || c == '\'' || c == '"' || c == ',' || c == '<' || c == '*' ||
		 c == '/' || c == '~' || c == '=' || c == '{' || c == '}' || c == '%' || c == '|' || c == '-' ||
		 c == 164 || // currency symbol
		 c == 176 || // degree symbol
		 c == 180 || // ACUTE ACCENT
		 c == 0xfe0f || // VARIATION SELECTOR-16
		 isSpace(c) || isSymbol(c) || isPrivate(c)
		 ) {
	if (current_token.size() >= 2) {
	  // cerr << "adding hashtag " << current_token << endl;
	  entities.push_back(current_token);
	}
	current_token.clear();
	if (isSpace(c)) {
	  state = STATE_INITIAL;
	} else {
	  state = STATE_WORD;
	}
      } else if (c == '#' || c == '@') {
	if (current_token.size() >= 2) {
	  // cerr << "adding hashtag " << current_token << endl;
	  entities.push_back(current_token);
	}
	current_token.clear();
	utf8::prior(str_i, str);
	state = STATE_INITIAL;
      } else {
	cerr << "invalid hashtag(" << c << "): " << input << "\n";
#if 0
	assert(0);
#else
	return set<string>();
#endif
      }
      break;
    case STATE_URL_HTTP1: case STATE_URL_HTTP2:
      if (c == 't' || c == 'T') {
	state++;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_FTP1:
      if (c == 't' || c == 'T') {
	state = STATE_URL_FTP2;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_FTP2: case STATE_URL_HTTP3:
      if (c == 'p' || c == 'P') {
	state++;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_HTTP4:
      if (c == ':') {
	current_token = "http:";
	state = STATE_URL_COLON;
      } else if (c == 's' || c == 'S') {
	state = STATE_URL_HTTP5;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }
      break;      
    case STATE_URL_FTP3:
      if (c == ':') {
	current_token = "ftp:";
	state = STATE_URL_COLON;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }     
      break;
    case STATE_URL_HTTP5:
      if (c == ':') {
	current_token = "https:";
	state = STATE_URL_COLON;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_COLON:
      if (c == '/') {
	current_token += "/";
	state = STATE_URL_SLASH;
      } else {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }      
      break;
    case STATE_URL_SLASH:
      if (c == '/') {
	current_token += '/';
	state = STATE_URL_BODY_START;
      } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c) || c == '.' || c == '-') {
	current_token += '/';
	utf8::append(c, back_inserter(current_token));
	state = STATE_URL_BODY;
      } else if (c == '[') {
	current_token += '/';
	current_token += '[';
	state = STATE_URL_BODY_IPV6;
      } else if (c == 173) { // soft hyphen
	// skip
      } else if (isSpace(c)) {
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      } else if (c == 8230) { // ellipsis
	state = STATE_WORD;
      } else {
	cerr << "A: failed to extract entities from text (c = " << c << "): " << input << endl;
#if 0
	assert(0);
#else
	return set<string>();
#endif
	state = STATE_WORD;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_BODY_START:
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c) || c == '.' || c == '-' || c == '/' || c >= 128) {
	utf8::append(c, back_inserter(current_token));
	state = STATE_URL_BODY;
      } else if (c == '[') {
	state = STATE_URL_BODY_IPV6;
      } else if (isSpace(c) || c == ')' || c == '_' || c == '<' || c == ',' || c == '\'') {
	state = STATE_WORD;
      } else if (c == 173) { // soft hyphen
	// skip
      } else if (c == ':' || c == '"' || c == '\'') { // trash
	// skip
      } else if (c == '&') {
	// This might be repaired, but it's too hard
	// http://&shy;&shy;&amp;shy;m.facebook.com/&shy;&shy;&amp;shy;anish.sangar.9?&shy;v&shy;=time&amp;shy;line&amp;amp&shy;;r&shy;efid=17
	state = STATE_WORD;
      } else {
	cerr << "invalid url body starting char '" << c << "': " << input << "\n";
#if 0
	assert(0);
#else
	return set<string>();
#endif
      }
      break;
    case STATE_URL_BODY:
      if (c == '&') {
	state = STATE_URL_AMP1;
      } else if (c == 'h') {
	state = STATE_URL_C_HTTP1;
      } else if (c == '=') {
	current_token += '=';	
	state = STATE_URL_ARG_START;
      } else if (c == '_') {
	current_token += '_';
	state = STATE_URL_UNDERSCORE;
      } else if (c == 173) { // soft hyphen
	// skip
      } else if (c == 28 || c == 31) { // Information separators
	// skip
      } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c) || (allow_utf8 && c > 127) || c == '.' || c == '%' || c == '/' || c == '?' || c == '~' || c == '-' || c == '_' || c == '=' || c == '!' || c == '+' || c == '@' || c =='\\' || c == '|' || c == '$' || c == '^' || c == '#') {
	// current_token += c;
	utf8::append(c, back_inserter(current_token));
      } else if (isSpace(c)) {
	entities.push_back(current_token);
	state = STATE_INITIAL;
      } else if ((!allow_utf8 && c > 127) || c == '\'' || c == '"' || c == '*' || c == '<' || c == '>' || c == '[' || c == ']' || c == '{' || c == '}' || c == '(' || c == ')' || c == ',' || c == '`') {
	entities.push_back(current_token);
	state = STATE_WORD;
	utf8::prior(str_i, str);
      } else if (c == ':') {
	state = STATE_SMILEY1_START;
      } else if (c == ';') {
	state = STATE_SMILEY2_START;      
      } else {
	cerr << "B: failed to extract entities from text (c = " << c << "): " << input << endl;
#if 0
	assert(0);
#else
	return set<string>();
#endif
      }
      break;
    case STATE_URL_BODY_IPV6:
      if (isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || c == '.' || c == ':') {
	utf8::append(c, back_inserter(current_token));
      } else if (c == ']') {
	current_token += ']';
	state = STATE_URL_BODY;
      } else {
	cerr << "invalid ipv6 url: " << input << endl;
#if 0
	assert(0);
#else
	return set<string>();
#endif
      }
      break;
    case STATE_URL_ARG_START:
      if (c == '{') {
	current_token += '{';
	state = STATE_URL_CU_BRACKET;
      } else if (c == '[') {
	current_token += '[';
	state = STATE_URL_SQ_BRACKET;
      } else if (c == '(') {
	state = STATE_URL_PARENS;
      } else {
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_UNDERSCORE:
      if (c == '(') {
	current_token += '(';
	state = STATE_URL_PARENS;
      } else {
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_CU_BRACKET:
      if (c == '}' || c == '&') {
	utf8::append(c, back_inserter(current_token));
	state = STATE_URL_BODY;
      } else if (c == '[' || c == '{' || c == '(') {
	cerr << "not implemented: " << input << "\n";
#if 0
	assert(0);
#else
	return set<string>();
#endif
      } else if (isSpace(c)) {
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      } else {
	// current_token += c;
	utf8::append(c, back_inserter(current_token));
      }
      break;
    case STATE_URL_SQ_BRACKET:
      if (c == ']' || c == '&') {
	utf8::append(c, back_inserter(current_token));
	state = STATE_URL_BODY;
      } else if (c == '[' || c == '{' || c == '(') {
	cerr << "not implemented: " << input << "\n";
#if 0
	assert(0);
#else
	return set<string>();
#endif
      } else if (isSpace(c)) {
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      } else {
	// current_token += c;
	utf8::append(c, back_inserter(current_token));
      }
      break;
    case STATE_URL_PARENS:
      if (c == ')') {
	current_token += ')';
	state = STATE_URL_BODY;
      } else if (c == '[' || c == '{' || c == '(') {
	cerr << "not implemented: " << input << "\n";
#if 0
	assert(0);	
#else
	return set<string>();
#endif
      } else if (isSpace(c)) {
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      } else {
	// current_token += c;
	utf8::append(c, back_inserter(current_token));
      }
      break;
    case STATE_URL_C_HTTP1:
      if (c == 't') {
	state++;
      } else {
	current_token += "h";
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_C_HTTP2:
      if (c == 't') {
	state++;
      } else {
	current_token += "ht";
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_C_HTTP3:
      if (c == 'p') {
	state++;
      } else {
	current_token += "htt";
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_C_HTTP4:
      if (c == ':') {
	entities.push_back(current_token);
	current_token = "http:";
	state = STATE_URL_COLON;
      } else if (c == 's') {
	state = STATE_URL_C_HTTP5;
      } else {
	current_token += "http";
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;      
    case STATE_URL_C_HTTP5:
      if (c == ':') {
	entities.push_back(current_token);
	current_token = "https:";
	state = STATE_URL_COLON;
      } else {
	current_token += "https";
	state = STATE_URL_BODY;
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_SMILEY1_START:
      if (c == ')' || c == '(') {
	entities.push_back(current_token);
	state = STATE_INITIAL;
	// i -= 2;
	utf8::prior(str_i, str);
	utf8::prior(str_i, str);
      } else {
	state = STATE_URL_BODY;
	current_token += ':';
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_SMILEY2_START:
      if (c == ')' || c == '(') {
	entities.push_back(current_token);
	state = STATE_INITIAL;
	// i -= 2;
	utf8::prior(str_i, str);
	utf8::prior(str_i, str);
      } else {
	state = STATE_URL_BODY;
	current_token += ';';
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_AMP1:
      if (c == 'a') {
	state = STATE_URL_AMP2;
      } else {
	state = STATE_URL_BODY;
	current_token += "&";
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_AMP2:
      if (c == 'm') {
	state = STATE_URL_AMP3;
      } else {
	state = STATE_URL_BODY;
	current_token += "&a";
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_AMP3:
      if (c == 'p') {
	state = STATE_URL_AMP4;
      } else {
	state = STATE_URL_BODY;
	current_token += "&am";
	// i--;
	utf8::prior(str_i, str);
      }
      break;
    case STATE_URL_AMP4:
      if (c == ';') {
	current_token += '&';
      } else {
	current_token += "&amp";
	// i--;
	utf8::prior(str_i, str);
      }
      state = STATE_URL_BODY;
      break;
    default:
#if 0
      assert(0);
#else
      return set<string>();
#endif
    }
  }
  if (state == STATE_URL_AMP1) {
    current_token += "&";
    state = STATE_URL_BODY;
  } else if (state == STATE_URL_AMP2) {
    current_token += "&a";
    state = STATE_URL_BODY;
  } else if (state == STATE_URL_AMP3) {
    current_token += "&am";
    state = STATE_URL_BODY;
  } else if (state == STATE_URL_AMP4) {
    current_token += "&amp";
    state = STATE_URL_BODY;
  } else if (state == STATE_SMILEY1_START) {
    current_token += ":";
    state = STATE_URL_BODY;
  } else if (state == STATE_SMILEY2_START) {
    current_token += ";";
    state = STATE_URL_BODY;
  } else if (state == STATE_URL_ARG_START || state == STATE_URL_CU_BRACKET || state == STATE_URL_SQ_BRACKET) {
    state = STATE_URL_BODY;
  }
  if (state == STATE_URL_BODY_IPV6) {
    cerr << "ipv6 url not finalized: " << input << "\n";
#if 0
    assert(0);
#endif
  }  
  if (state == STATE_URL_BODY || state == STATE_MENTION_IDEOGRAPH || state == STATE_MENTION_LATIN || state == STATE_HASHTAG) {
    entities.push_back(current_token);	
  }
  set<string> results;
  for (vector<string>::iterator it = entities.begin(); it != entities.end(); it++) {
    string & s = *it;
    if (((s[0] == '#' || s[0] == '@')) && s.size() >= 3) {
      bool has_alpha = false;
      const char * str = s.c_str(), * str_i = s.c_str(), * end = s.c_str() + s.size();
      while ( str_i < end ) {
	uint32_t c = utf8::next(str_i, end);
	if (isAlpha(c) || isIdeograph(c)) {
	  has_alpha = true;
	}
      }
      if (has_alpha) {
	string s2 = s;
	string tmp = StringUtils::toLower(s2);
	if (tmp != "#google+" && !(tmp.size() == 3 && tmp[0] == '#' && isalpha(tmp[1]) && tmp[2] == '+')) {
	  while (s2.back() == '&' || s2.back() == '+' || s2.back() == '-') {
	    s2.erase(s2.size() - 1);
	  }
	}
	if (s2.size() >= 3) {
	  results.insert(s2);
	  // cerr << "keeping entity " << s2 << endl;
	}
      }
    } else {
      while ( s.size() ) {
	if (s.back() == ';' || s.back() == '(' || s.back() == '.' || s.back() == ',' || s.back() == '$' || s.back() == '!' || s.back() == '?' || s.back() < 0) {
	  // cerr << "cutting " << int(s.back()) << endl;
	  s.erase(s.size() - 1);
	} else {
	  // cerr << "NOT cutting " << int(s.back()) << endl;
	  break;
	}
      }

      bool has_host = false, has_dot = false, has_tld = false;
      for (unsigned int i = 0; i < s.size(); i++) {
	if (!has_host && isalnum(s[i])) {
	  has_host = true;
	} else if (has_host && !has_dot && s[i] == '.') {
	  has_dot = true;
	} else if (has_host && has_dot && !has_tld && isAlpha(s[i])) {
	  has_tld = true;
	}
      }
      if (has_host && has_dot && has_tld) {
	results.insert(URI::urlencodeUtf8(*it));
      }
    }
  }
  return results;
}

string
StringUtils::repairUTF8(const string & s) {
  string s2;
  for (unsigned int i = 0; i < s.size(); i++) {
    if (i + 1 < s.size() && s[i] == '\xc3' && s[i + 1] == '\x3f') {
      // ?
      s2 += "\xc3\x84";
      i++;
    } else if (i + 8 < s.size() && s[i] == '\xc3' && s[i + 1] == '\x83' && s[i + 2] == '\xe2' && s[i + 3] == '\x80' && s[i + 4] == '\x9e' && s[i + 5] == '\xC3' && s[i + 6] == '\x83' && s[i + 7] == '\xc2' && s[i + 8] == '\xa4') {
      // Äänekoski -korjaus clientLocations taululle
      s2 += "\xc3\x84\xc3\xa4";
      i += 8;
    } else {
      s2 += s[i];
    }
  }
  return s2;
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
    if (isAlpha(lcc) && lcc == prev_lcc) {
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
StringUtils::isWord(const string & input) {
  if (!input.empty()) {
    const char * str = input.c_str();
    const char * str_i = str;
    const char * end = str + input.size(); 
    while ( str_i < end ) {
      uint32_t c = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
      if (isUpper(c) || isLower(c)) {
	return true;
      }
    }
  }
  return false;
}
