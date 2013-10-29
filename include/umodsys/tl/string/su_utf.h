#ifndef __UMODSYS_TYPES_STRING_UTILS_UTF_H
#define __UMODSYS_TYPES_STRING_UTILS_UTF_H 1
/*************************************************************/
// file: umodsys/tl/string/su_utf.h
// info: string utilities utf8, utf16. utf32
/*************************************************************/

#include <umodsys/tl/string/su_typedefs.h>

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

//***************************************
// utf32 <=> utf8[] conversion
//***************************************

size_t utf_32to8(utf8* dest, size_t destmax, const utf32* src, size_t srclen, bool gracial=true);
size_t utf_32to8_len(const utf32* src, size_t srclen);
size_t utf_8to32(utf32* dest, size_t destmax, const utf8* src, size_t srclen, bool gracial=true);
size_t utf_8to32_len(const utf8* src, size_t srclen);

inline size_t utf_32to8(utf8* dest, size_t destmax, utf32 src)
{
  register utf32 ch = src;
  register unsigned char bytesToWrite = 0;
  const utf8 byteMask = utf8(0xBF);
  const utf8 byteMark = utf8(0x80);

  if (ch < 0x80)              bytesToWrite = 1;
  else if (ch < 0x800)        bytesToWrite = 2;
  else if (ch < 0x10000)      bytesToWrite = 3;
  else if (ch < 0x200000)     bytesToWrite = 4;
  else if (ch < 0x4000000)    bytesToWrite = 5;
  else if (ch <= utf_kMaximumUCS4) bytesToWrite = 6;
  else { bytesToWrite = 2; ch = utf_kReplacementCharacter; }

  if(destmax<bytesToWrite)
    return 0;
  switch (bytesToWrite) { /* note: code falls through cases! */
    case 6: dest[5] = (utf8(ch) | byteMark) & byteMask; ch >>= 6;
    case 5: dest[4] = (utf8(ch) | byteMark) & byteMask; ch >>= 6;
    case 4: dest[3] = (utf8(ch) | byteMark) & byteMask; ch >>= 6;
    case 3: dest[2] = (utf8(ch) | byteMark) & byteMask; ch >>= 6;
    case 2: dest[1] = (utf8(ch) | byteMark) & byteMask; ch >>= 6;
    case 1: dest[0] =  utf8(ch) | utf_firstByteMark[bytesToWrite];
  };
  return bytesToWrite;
}

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

inline bool utf_8to32(utf32 &dest, const utf8* &src, size_t &srcsize)
{
  register utf32 ch = 0;
  if(srcsize<=0)
    return false;
  register unsigned char extraBytesToWrite = utf_bytesFromUTF8[utf8u(*src)];
  if(srcsize<extraBytesToWrite+1)
    return false;
  switch(extraBytesToWrite) { /* note: code falls through cases! */
    case 5: ch += utf8u(*src++); ch <<= 6;
    case 4: ch += utf8u(*src++); ch <<= 6;
    case 3: ch += utf8u(*src++); ch <<= 6;
    case 2: ch += utf8u(*src++); ch <<= 6;
    case 1: ch += utf8u(*src++); ch <<= 6;
    case 0: ch += utf8u(*src++);
  };
  srcsize -= extraBytesToWrite+1;
  ch -= utf_offsetsFromUTF8[extraBytesToWrite];
  if(ch > utf_kMaximumUCS4) {
    dest = utf_kReplacementCharacter;
    return true;
  }
  dest = ch;
  return true;
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
// end

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_UTF_H
