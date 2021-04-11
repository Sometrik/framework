#ifndef _CHARACTERTESTS_H_
#define _CHARACTERTESTS_H_

#include <stdint.h>

static inline bool is_upper_unicode(uint32_t c) {
  if (c >= 'A' && c <= 'Z') {
    return true;
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

static inline bool is_lower_unicode(uint32_t c) {
  if (c >= 'a' && c <= 'z') {
    return true;
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

static inline bool is_alpha_unicode(uint32_t cp) {
  if (is_lower_unicode(cp) || is_upper_unicode(cp)) {
    return true;
  } else if (cp >= 0x05d0 && cp <= 0x05ea) { // Hebrew letters
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
  } else if (cp >= 0x0900 && cp <= 0x097f) { // Devanagari
    return true;
  } else if (cp >= 0x0980 && cp <= 0x09ff) { // Bengali
    return true;
  } else if (cp >= 0x0a00 && cp <= 0x0a7f) { // Gurmukhi
    return true;
  } else if (cp >= 0x0a80 && cp <= 0x0aff) { // Gujarati
    return true;
  } else if (cp >= 0x0b00 && cp <= 0x0b7f) { // Oriya
    return true;
  } else if (cp >= 0x0b80 && cp <= 0x0bff) { // Tamil
    return true;
  } else if (cp >= 0x0c00 && cp <= 0x0c7f) { // Telugu
    return true;
  } else if (cp >= 0x0c80 && cp <= 0x0cff) { // Kannada
    return true;
  } else if (cp >= 0x0d00 && cp <= 0x0d7f) { // Malayalam
    return true;
  } else if (cp >= 0x0E00 && cp <= 0x0E7F) { // Thai
    return true;
  } else if (cp >= 0x1E00 && cp <= 0x1EFF) { // Latin Extended Additional
    return true;
  }
  return false;
}

static inline bool is_complex_symbol_unicode(uint32_t cp) {
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
  } else if (cp >= 0x1F900 && cp <= 0x1F9FF) { // Supplemental Symbols and Pictographs
    return true;
  } else {
    return false;
  }
}

static inline bool is_syllabary_unicode(uint32_t cp) {
  if ((cp >= 0x3040 && cp <= 0x309f) || // Hiragana
      (cp >= 0x30a0 && cp <= 0x30ff) || // Katakana
      (cp >= 0x31f0 && cp <= 0x31ff) || // Katakana phonetic extensions
      (cp >= 0xAC00 && cp <= 0xD7AF) || // Hangul Syllables
      (cp >= 0x1100 && cp <= 0x1112) || // Hangul jamos
      (cp >= 0x1161 && cp <= 0x1175) || // Hangul jamos
      (cp >= 0x11a8 && cp <= 0x11c2) // Hangul jamos      
      ) {
    return true;
  } else {
    return false;
  }
}

static inline bool is_ideograph_unicode(uint32_t cp) {
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

static inline bool is_arabic_unicode(uint32_t cp) {
  if (cp >= 0xFE70 && cp <= 0xFEFF) { // Arabic Presentation Forms-B
    return true;
  } else {
    return false;
  }
}

static inline bool is_symbol_unicode(uint32_t cp) {
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
  } else if (is_complex_symbol_unicode(cp)) {
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

static inline bool is_combining_unicode(uint32_t cp) {
  if (cp == 0x032b) { // COMBINING INVERTED DOUBLE ARCH BELOW
    return true;
  } else {
    return false;
  }
}

static inline bool is_private_unicode(uint32_t cp) {
  if (cp >= 0xE000 && cp <= 0xF8FF) {
    return true;
  } else {
    return false;
  }
}

static inline bool is_digit_unicode(uint32_t c) {
  return c >= '0' && c <= '9';
}

static inline bool is_blank_unicode(uint32_t c ) {
  switch (c) {
  case ' ': return true; // space
  case '\t': return true; // tab
  case 8194: return true; // en space
  case 8195: return true; // em space
  case 8201: return true; // THIN SPACE
  case 160: return true; // nbsp
  case 0x3000: return true; // ideographic space
  default: return false;
  }
}

static inline bool is_control_unicode(uint32_t c) {
  return c == 0xad || // soft hyphen
    c == 0x200b || // ZERO WIDTH SPACE
    c == 0x200e || // LEFT-TO-RIGHT MARK
    c == 0x202c || // POP DIRECTIONAL FORMATTING
    c == 0x2003 ||
    c == 0x202c || // LEFT-TO-RIGHT MARK / POP DIRECTIONAL FORMATTING
    c == 0xfeff; // Zero width no-break space
}

static inline bool is_invalid_unicode(uint32_t c) {
  return c == 0xfefe || c == 0xfffe;
}

static inline bool is_space_unicode(uint32_t cp) { 
  return is_blank_unicode(cp) ||
    is_control_unicode(cp) ||
    cp == '\b' ||
    cp == 0x0a ||
    cp == 0x0d;    
}

#endif
