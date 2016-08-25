#include "umodsys/tl/string/strings.h"

using namespace UModSys;
using namespace tl;
using namespace su;

//***************************************
// namespace su::
//***************************************

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
// utf32[] <=> utf8[] conversion
//***************************************

size_t su::utf_32to8(utf8* dest, size_t destmax, utf32 src)
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

bool su::utf_8to32(utf32 &dest, const utf8* &src, size_t &srcsize)
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
// ::
//***************************************
