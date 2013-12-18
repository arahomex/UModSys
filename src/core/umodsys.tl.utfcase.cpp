#include "umodsys/tl/string/strings.h"

using namespace UModSys;
using namespace tl;
using namespace su;

//***************************************
// comparission nocase
//***************************************

#define UTF32 su::utf32
#define FAST_U_VAL su::utf_nocase32
#include "utf8_case_casefast.h"
#undef FAST_U_VAL
#undef UTF32

//***************************************
//***************************************

int su::utf32_shash_nocase_bin(const utf32* str, size_t len)
{
  register unsigned a=0, b=0;
  while(len>0) {
    a += utf_nocase32(*str++);
    b += a & 0xffff;
    len--;
  }
  return ((b<<8)&0xff) | a;
}

int su::utf32_cmp_nocase_bin(const utf32* a, const utf32 *b, size_t num)
{
  for(; num>0; num--) {
    utf32 xa = utf_nocase32(*a++), xb = utf_nocase32(*b++);
    if(xa!=xb)
      return (xa>xb) - (xa<xb);
  }
  return 0;
}

//***************************************
//***************************************

int su::utf16_shash_nocase_bin(const utf16* str, size_t len)
{
  register unsigned a=0, b=0;
  while(1) {
    utf32 xa;
    if(!utf_16to32(xa, str, len))
      break;
    a += utf_nocase32(xa);
    b += a & 0xffff;
  }
  return ((b<<8)&0xff) | a;
}

int su::utf16_cmp_nocase_bin(const utf16* a, const utf16 *b, size_t sa, size_t sb)
{
  while(sa && sb) {
    utf32 xa, xb;
    bool ra = utf_16to32(xa, a, sa), rb = utf_16to32(xb, b, sb);
    if(ra!=rb)
      return (ra>rb) - (ra<rb);
    xa = utf_nocase32(xa); xb = utf_nocase32(xb);
    if(xa!=xb)
      return (xa>xb) - (xa<xb);
  }
  return 0;
}

//***************************************
//***************************************

int su::utf8_shash_nocase_bin(const utf8* str, size_t len)
{
  register unsigned a=0, b=0;
  while(1) {
    utf32 xa;
    if(!utf_8to32(xa, str, len))
      break;
    a += utf_nocase32(xa);
    b += a & 0xffff;
  }
  return ((b<<8)&0xff) | a;
}

int su::utf8_cmp_nocase_bin(const utf8* a, const utf8 *b, size_t sa, size_t sb)
{
  while(sa && sb) {
    utf32 xa, xb;
    bool ra = utf_8to32(xa, a, sa), rb = utf_8to32(xb, b, sb);
    if(ra!=rb)
      return (ra>rb) - (ra<rb);
    xa = utf_nocase32(xa); xb = utf_nocase32(xb);
    if(xa!=xb)
      return (xa>xb) - (xa<xb);
  }
  return 0;
}

//***************************************
// ::
//***************************************
