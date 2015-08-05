#ifndef __UMODSYS_TYPES_STRING_UTILS_UTF_H
#define __UMODSYS_TYPES_STRING_UTILS_UTF_H 1
/*************************************************************/
// file: umodsys/tl/string/su_utf.h
// info: string utilities utf8, utf16. utf32
/*************************************************************/

#include <umodsys/tl/string/su_typedefs.h>
#include <umodsys/tl/string/su_char.h>
#include <umodsys/tl/string/su_charw.h>
#include <umodsys/tl/string/su_charl.h>

namespace UModSys {
namespace tl {
namespace su {

//***************************************
// UTF types
//***************************************

typedef core::BChar       utf8;
typedef core::BCharIndex  utf8u;
typedef core::BCharW      utf16;
typedef core::BCharL      utf32;

//***************************************
// UTF constants
//***************************************

extern const utf8u utf_bytesFromUTF8[256];
extern const utf8u utf_firstByteMark[7];
extern const utf32 utf_offsetsFromUTF8[6];

const char utf_max8len = 6;
const char utf_max16len = 2;

const char  utf_halfShift            = 10;
const utf32 utf_halfBase             = 0x0010000UL;
const utf32 utf_halfMask             = 0x3FFUL;
const utf32 utf_kSurrogateHighStart  = 0xD800UL;
const utf32 utf_kSurrogateHighEnd    = 0xDBFFUL;
const utf32 utf_kSurrogateLowStart   = 0xDC00UL;
const utf32 utf_kSurrogateLowEnd     = 0xDFFFUL;
const utf32 utf_kReplacementCharacter=	0x0000FFFDUL;
const utf32 utf_kMaximumUCS2         =	0x0000FFFFUL;
const utf32 utf_kMaximumUTF16        =	0x0010FFFFUL;
const utf32 utf_kMaximumUCS4         =	0x7FFFFFFFUL;
const utf32 utf_kSurrogateOffset     = utf_halfBase - (utf_kSurrogateHighStart << 10) - utf_kSurrogateLowStart;

//***************************************
// utf16[] <=> utf8[] conversion
//***************************************

size_t utf_16to8(utf8* dest, size_t destmax, const utf16* src, size_t srclen, bool gracial=true);
size_t utf_16to8_len(const utf16* src, size_t srclen);
size_t utf_8to16(utf16* dest, size_t destmax, const utf8* src, size_t srclen, bool gracial=true);
size_t utf_8to16_len(const utf8* src, size_t srclen);

utf32 utf_nocase32(utf32 v);

//***************************************
// utf32 <=> utf8[] conversion
//***************************************

size_t utf_32to8(utf8* dest, size_t destmax, const utf32* src, size_t srclen, bool gracial=true);
size_t utf_32to8_len(const utf32* src, size_t srclen);
size_t utf_8to32(utf32* dest, size_t destmax, const utf8* src, size_t srclen, bool gracial=true);
size_t utf_8to32_len(const utf8* src, size_t srclen);
size_t utf_32to8(utf8* dest, size_t destmax, utf32 src);
bool utf_8to32(utf32 &dest, const utf8* &src, size_t &srcsize);

inline size_t utf_32to8_len(utf32 src)
{
  register utf32 ch = src;
  if (ch < 0x80) return 1;
  else if (ch < 0x800) return 2;
  else if (ch < 0x10000) return 3;
  else if (ch < 0x200000) return 4;
  else if (ch < 0x4000000) return 5;
  else if (ch <= utf_kMaximumUCS4) return 6;
  else return 2;
}

//***************************************
// utf32 <=> utf16[] conversion
//***************************************

size_t utf_32to16(utf16* dest, size_t destmax, const utf32* src, size_t srclen, bool gracial=true);
size_t utf_32to16_len(const utf32* src, size_t srclen);
size_t utf_16to32(utf32* dest, size_t destmax, const utf16* src, size_t srclen, bool gracial=true);
size_t utf_16to32_len(const utf16* src, size_t srclen);

//***************************************

inline size_t utf_32to16(utf16* dest, size_t destmax, utf32 src)
{
  if(src>utf_kMaximumUCS2) {
    if(destmax<2)
      return 0;
    src -= utf_halfBase;
    dest[0] = utf_kSurrogateHighStart + (src >> utf_halfShift);
    dest[1] = utf_kSurrogateLowStart + (src & utf_halfMask);
    return 2;
  }
  if(src>utf_kMaximumUTF16)
    src = utf_kReplacementCharacter;  // out of UTF character
  else if(src>=utf_kSurrogateHighStart && src<=utf_kSurrogateHighEnd)
    src = utf_kReplacementCharacter;  // surrogate is not allowed
  else if(src>=utf_kSurrogateLowStart && src<=utf_kSurrogateLowEnd)
    src = utf_kReplacementCharacter; // surrogate is not allowed
  if(destmax<1)
    return 0;
  dest[0] = src;
  return 1;
}

inline size_t utf_32to16_len(utf32 src)
{
  if(src>utf_kMaximumUCS2)
    return 2;
  return 1;
}

//***************************************

inline bool utf_16to32(utf32 &dest, const utf16* &src, size_t &srcsize)
{
  if(srcsize<=0)
    return false;
  register utf16 ch = *src++;
  srcsize--;
  if (ch >= utf_kSurrogateHighStart && ch <= utf_kSurrogateHighEnd) {
    if(srcsize==0)
      return false; // Trailing surrogate missing from UTF-16 sequence
    register utf16 ch2 = *src++;
    srcsize--;
    if(ch2<utf_kSurrogateLowStart || ch2>utf_kSurrogateLowEnd)
      return false; // Trailing character in a UTF-16 surrogate pair is missing (outside correct range)
    dest = (ch<<utf_halfShift) + ch2 + utf_kSurrogateOffset;
    return true;
  }
  dest = ch;
  return true;
}

//***************************************
// utf32 nocase
//***************************************

size_t utf32_shash_nocase_bin(const utf32* str, size_t len);
int utf32_cmp_nocase_bin(const utf32* a, const utf32 *b, size_t num);

inline int utf32_cmp_nocase_bin(const utf32* a, const utf32 *b, size_t sa, size_t sb)
{
  if(sa<=sb) {
    int rv = utf32_cmp_nocase_bin(a, b, sa);
    if(rv!=0 || sa==sb)
      return rv;
    return -1;
  } else {
    int rv = utf32_cmp_nocase_bin(a, b, sb);
    if(rv!=0)
      return rv;
    return 1;
  }
}

inline int utf32_cmp_nocase(const utf32* a, const utf32 *b)
{
  if(a==NULL) return b!=NULL ? 1 : 0; else if(b==NULL) return -1;
  size_t sa = su::slen(a), sb = su::slen(b);
  if(sa!=sb) return (sa>sb)-(sa<sb);
  return utf32_cmp_nocase_bin(a, b, sa);
}

inline int utf32_cmp_nocase(const utf32* a, const utf32 *b, size_t num)
{
  if(a==NULL) return b!=NULL ? 1 : 0; else if(b==NULL) return -1;
  size_t sa = su::slen(a, num), sb = su::slen(b, num);
  if(sa!=sb) return (sa>sb)-(sa<sb);
  return utf32_cmp_nocase_bin(a, b, sa);
}

//***************************************
// utf16 nocase
//***************************************

size_t utf16_shash_nocase_bin(const utf16* str, size_t len);
int utf16_cmp_nocase_bin(const utf16* a, const utf16 *b, size_t sa, size_t sb);

inline int utf16_cmp_nocase(const utf16* a, const utf16 *b)
{
  if(a==NULL) return b!=NULL ? 1 : 0; else if(b==NULL) return -1;
  return utf16_cmp_nocase_bin(a, b, su::slen(a), su::slen(b));
}

inline int utf16_cmp_nocase(const utf16* a, const utf16 *b, size_t num)
{
  if(a==NULL) return b!=NULL ? 1 : 0; else if(b==NULL) return -1;
  return utf16_cmp_nocase_bin(a, b, su::slen(a, num), su::slen(b, num));
}

//***************************************
// utf8 nocase
//***************************************

size_t utf8_shash_nocase_bin(const utf8* str, size_t len);
int utf8_cmp_nocase_bin(const utf8* a, const utf8 *b, size_t sa, size_t sb);

inline int utf8_cmp_nocase(const utf8* a, const utf8 *b)
{
  if(a==NULL) return b!=NULL ? 1 : 0; else if(b==NULL) return -1;
  return utf8_cmp_nocase_bin(a, b, su::slen(a), su::slen(b));
}

inline int utf8_cmp_nocase(const utf8* a, const utf8 *b, size_t num)
{
  if(a==NULL) return b!=NULL ? 1 : 0; else if(b==NULL) return -1;
  return utf8_cmp_nocase_bin(a, b, su::slen(a, num), su::slen(b, num));
}

//***************************************
// nocase
//***************************************

template<typename T> size_t utf_shash_nocase(const T* str, size_t len);
template<typename T> int utf_cmp_nocase(const T* a, size_t sa, const T *b, size_t sb);
template<typename T> int utf_cmp_nocase(const T* a, const T *b, size_t sab);
template<typename T> int utf_cmp_nocase(const T* a, size_t sab, const T *b);

template<> inline size_t utf_shash_nocase<utf8>(const utf8* str, size_t len) { return utf8_shash_nocase_bin(str, len); }
template<> inline size_t utf_shash_nocase<utf16>(const utf16* str, size_t len) { return utf16_shash_nocase_bin(str, len); }
template<> inline size_t utf_shash_nocase<utf32>(const utf32* str, size_t len) { return utf32_shash_nocase_bin(str, len); }

template<> inline int utf_cmp_nocase<utf8>(const utf8* a, size_t sa, const utf8* b, size_t sb) { return utf8_cmp_nocase_bin(a, b, sa, sb); }
template<> inline int utf_cmp_nocase<utf16>(const utf16* a, size_t sa, const utf16* b, size_t sb) { return utf16_cmp_nocase_bin(a, b, sa, sb); }
template<> inline int utf_cmp_nocase<utf32>(const utf32* a, size_t sa, const utf32* b, size_t sb) { return utf32_cmp_nocase_bin(a, b, sa, sb); }

template<> inline int utf_cmp_nocase<utf8>(const utf8* a, const utf8* b, size_t sab) { return utf8_cmp_nocase_bin(a, b, sab, sab); }
template<> inline int utf_cmp_nocase<utf16>(const utf16* a, const utf16* b, size_t sab) { return utf16_cmp_nocase_bin(a, b, sab, sab); }
template<> inline int utf_cmp_nocase<utf32>(const utf32* a, const utf32* b, size_t sab) { return utf32_cmp_nocase_bin(a, b, sab, sab); }


template<typename CharT>
inline size_t utf_shash_nocase(const CharT* str) 
{
  if(str==NULL || *str==0)
    return 0;
  return utf_shash_nocase_bin(str, slen(str));
}

template<typename CharT>
inline size_t utf_shash_nocase(const CharT* str, size_t len) {
  if(str==NULL || *str==0 || len==0)
    return 0;
  return utf_shash_nocase_bin(str, len);
}

template<typename CharT>
inline size_t utf_shash_nocase(const CharT* str, size_t* len) {
  *len = 0;
  if(str==NULL || *str==0)
    return 0;
  return utf_shash_nocase_bin(str, *len=slen(str));
}

//***************************************
// end

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_UTF_H
