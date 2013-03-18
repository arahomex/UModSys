#ifndef __UMODSYS_TYPES_STRING_UTILS_BASE_H
#define __UMODSYS_TYPES_STRING_UTILS_BASE_H 1
/*************************************************************/
// file: umodsys/tl/string/su_base.h
// info: string utilities base
/*************************************************************/

#include <umodsys/tl/string/su_typedefs.h>

//#define S_STRING_COLLATE_CON const ICharCollate &col=su::collate()


namespace UModSys {
namespace tl {
namespace su {

//***************************************

#define UMODSYS_STRING_CLASS_HEADER(CharT) \
  typedef su::TCharOptions<CharT>       Options; \
  typedef su::TSCoreConst<CharT>        ConstStr;\
  typedef su::TSCoreBuffer<CharT>       BufferStr;\
  typedef typename Options::Char        Char; \
  typedef typename Options::Index       Index; \
  typedef typename Options::CStr        Str; \
  typedef typename Options::OStr        OStr; \

/*
  typedef TLString<CharT>           LString; \
  typedef TLHString<CharT>          HString; \
  typedef TLHAString<CharT>         AString; \
  typedef TLHSString<CharT>         SString; \
*/

//***************************************
// Char options

template<typename CharT>
struct TCharOptions {
  typedef CharT           Char;
  typedef CharT           Index;
  typedef const CharT*    CStr;
  typedef CharT*          OStr;
};

//***************************************
// string processor functions

template<typename CharT>
size_t slen(const CharT* s);

template<typename CharT>
size_t slen(const CharT* s, size_t maxn);

template<typename CharT>
void scpy(CharT* x, const CharT* s);

template<typename CharT>
void scpy(CharT* x, const CharT* s, size_t maxn);

template<typename CharT>
int scmp(const CharT* a, const CharT* b);

template<typename CharT>
int scmp(const CharT* a, const CharT* b, size_t maxn);

template<typename CharT>
int scmp(const CharT* a, size_t la, const CharT* b);

template<typename CharT>
bool seq(const CharT* a, const CharT* b);

template<typename CharT>
bool seq(const CharT* a, const CharT* b, size_t maxn);

template<typename CharT>
bool seq(const CharT* a, size_t la, const CharT* b);

template<typename CharT>
const CharT* schr(const CharT* a, CharT ch);

template<typename CharT>
const CharT* schr(const CharT* a, CharT ch, size_t maxn);

template<typename CharT>
const CharT* sstr(const CharT* a, const CharT* ch);

template<typename CharT>
const CharT* sstr(const CharT* a, const CharT* ch, size_t maxn);

template<typename CharT>
inline void scpyz(CharT* x, const CharT* s, size_t maxn) 
{ 
  scpy(x, s, maxn); 
  x[maxn]=0; 
}


template<typename CharT>
inline void smemcpy(CharT* x, const CharT* s, size_t n)
{
  ::memcpy(x, s, sizeof(CharT)*n);
}

template<typename CharT>
void smemmove(CharT* x, const CharT* s, size_t n)
{
  ::memmove(x, s, sizeof(CharT)*n);
}

template<typename CharT>
int shash(const CharT* str) 
{
  if(str==NULL || *str==0)
    return 0;
  register unsigned a=0, b=0;
  while(*str) {
    a += static_cast<typename TCharOptions<CharT>::Index>(*str++);
    b += a & 0xff;
  }
  return ((b<<8)&0xff) | a;
}

template<typename CharT>
int shash(const CharT* str, size_t len) {
  if(str==NULL || *str==0)
    return 0;
  register unsigned a=0, b=0;
  while(*str && len>0) {
    a += static_cast<typename TCharOptions<CharT>::Index>(*str++);
    b += a & 0xff;
    len--;
  }
  return ((b<<8)&0xff) | a;
}

template<typename CharT>
int shashlen(const CharT* str, size_t& len) {
  len = 0;
  if(str==NULL || *str==0)
    return 0;
  register unsigned a=0, b=0;
  while(*str) {
    a += static_cast<typename TCharOptions<CharT>::Index>(*str++);
    b += a & 0xff;
    len++;
  }
  return ((b<<8)&0xff) | a;
}

template<typename CharT>
int wildcmp(const CharT* wild, const CharT* string) 
{
  register const CharT* cp, *mp;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }
  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }
  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

//***************************************
// end

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_BASE_H
