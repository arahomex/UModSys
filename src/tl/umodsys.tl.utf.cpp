#include "umodsys/tl/string/strings.h"

using namespace UModSys;
using namespace tl;
using namespace su;

//***************************************
// namespace su::
//***************************************

const su::utf8u su::utf_bytesFromUTF8[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

const su::utf8u su::utf_firstByteMark[7] = {
  0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};

const su::utf32 su::utf_offsetsFromUTF8[6] =	{
  0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

//***************************************
// comparission nocase
//***************************************

int su::utf8_cmp_nocase(const BChar* a, const BChar *b)
{
  return ::strcmp(a, b);
}

int su::utf8_cmp_nocase(const BChar* a, const BChar *b, size_t num)
{
  return ::strncmp(a, b, num);
}

//***************************************
// utf16[] <=> utf8[] conversion
//***************************************

size_t su::utf_16to8(utf8* dest, size_t destmax, const utf16* src, size_t srclen, bool gracial)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_16to32(tmp, src, srclen)) {
    int n = utf_32to8(dest, destmax, tmp);
    if(n==0)
      return gracial ? rv : 0;
    rv += n;
    dest += n;
    destmax -= n;
  }
  return rv;
}

size_t su::utf_16to8_len(const utf16* src, size_t srclen)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_16to32(tmp, src, srclen)) {
    int n = utf_32to8_len(tmp);
    rv += n;
  }
  return rv;
}

//***************************************

size_t su::utf_8to16(utf16* dest, size_t destmax, const utf8* src, size_t srclen, bool gracial)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_8to32(tmp, src, srclen)) {
    int n = utf_32to16(dest, destmax, tmp);
    if(n==0)
      return gracial ? rv : 0;
    rv += n;
    dest += n;
    destmax -= n;
  }
  return rv;
}

size_t su::utf_8to16_len(const utf8* src, size_t srclen)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_8to32(tmp, src, srclen)) {
    int n = utf_32to16_len(tmp);
    rv += n;
  }
  return rv;
}

//***************************************
// utf32[] <=> utf8[] conversion
//***************************************

size_t su::utf_32to8(utf8* dest, size_t destmax, const utf32* src, size_t srclen, bool gracial)
{
  size_t rv = 0;
  for(;srclen>0; src++,srclen--) {
    int n = utf_32to8(dest, destmax, *src);
    if(n==0)
      return gracial ? rv : 0;
    rv += n;
    dest += n;
    destmax -= n;
  }
  return rv;
}

size_t su::utf_32to8_len(const utf32* src, size_t srclen)
{
  size_t rv = 0;
  for(;srclen>0; src++,srclen--) {
    int n = utf_32to8_len(*src);
    rv += n;
  }
  return rv;
}

//***************************************

size_t su::utf_8to32(utf32* dest, size_t destmax, const utf8* src, size_t srclen, bool gracial)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_8to32(tmp, src, srclen)) {
    if(destmax<=0)
      return gracial ? rv : 0;
    rv ++;
    *dest++ = tmp;
    destmax --;
  }
  return rv;
}

size_t su::utf_8to32_len(const utf8* src, size_t srclen)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_8to32(tmp, src, srclen)) {
    rv++;
  }
  return rv;
}

//***************************************
// utf32[] <=> utf16[] conversion
//***************************************

size_t su::utf_32to16(utf16* dest, size_t destmax, const utf32* src, size_t srclen, bool gracial)
{
  size_t rv = 0;
  for(;srclen>0; src++,srclen--) {
    int n = utf_32to16(dest, destmax, *src);
    if(n==0)
      return gracial ? rv : 0;
    rv += n;
    dest += n;
    destmax -= n;
  }
  return rv;
}

size_t su::utf_32to16_len(const utf32* src, size_t srclen)
{
  size_t rv = 0;
  for(;srclen>0; src++,srclen--) {
    int n = utf_32to16_len(*src);
    rv += n;
  }
  return rv;
}

//***************************************

size_t su::utf_16to32(utf32* dest, size_t destmax, const utf16* src, size_t srclen, bool gracial)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_16to32(tmp, src, srclen)) {
    if(destmax<=0)
      return gracial ? rv : 0;
    rv ++;
    *dest++ = tmp;
    destmax --;
  }
  return rv;
}

size_t su::utf_16to32_len(const utf16* src, size_t srclen)
{
  utf32 tmp;
  size_t rv = 0;
  while(utf_16to32(tmp, src, srclen)) {
    rv++;
  }
  return rv;
}

//***************************************
// ::
//***************************************
