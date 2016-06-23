#include "umodsys/tl/string/strings.h"

using namespace UModSys;
using namespace tl;
using namespace su;

//***************************************
// namespace su::
//***************************************

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
