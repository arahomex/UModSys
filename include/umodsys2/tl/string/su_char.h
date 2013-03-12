#ifndef __NANOUTL_TYPES_STRUTILS_CHAR_H
#define __NANOUTL_TYPES_STRUTILS_CHAR_H 1
//*****************************************************
// nanoutl/types/su_char.h: string utilities [char]
//*****************************************************

#include <nanoutl/types/su_base.h>

namespace NanoUTL {
namespace su {

template<>
struct TCharOptions<core::BChar> {
  typedef core::BChar        Char;
  typedef core::BCharIndex   Index;
  typedef core::BCStr        CStr;
  typedef core::BOStr        OStr;
  //
};

template<>
inline size_t slen(const core::BChar* s)
{ 
  return ::strlen(s);
}

template<>
inline size_t slen(const core::BChar* s, size_t maxn)
{
  const core::BChar* x = reinterpret_cast<const core::BChar*>(::memchr(s, 0, maxn)); 
  return x==NULL ? maxn : x-s; 
}

template<>
inline void scpy(core::BChar* x, const core::BChar* s)
{ 
  ::strcpy(x, s); 
}

template<>
inline void scpy(core::BChar* x, const core::BChar* s, size_t maxn)
{ 
  ::strncpy(x, s, maxn); 
}

template<>
inline int scmp(const core::BChar* a, const core::BChar* b)
{
  return ::strcmp(a, b);
}

template<>
inline int scmp(const core::BChar* a, const core::BChar* b, size_t maxn)
{
  return ::strncmp(a, b, maxn);
}

template<>
inline int scmp(const core::BChar* a, size_t la, const core::BChar* b)
{
  int rv = ::strncmp(a, b, la);
  return rv ? rv : core::scalar_compare(core::BCharIndex(0), core::BCharIndex(b[la]));
}

template<>
inline bool seq(const core::BChar* a, const core::BChar* b)
{
  return ::strcmp(a, b)==0;
}

template<>
inline bool seq(const core::BChar* a, const core::BChar* b, size_t maxn)
{
  return ::strncmp(a, b, maxn)==0;
}

template<>
inline bool seq(const core::BChar* a, size_t la, const core::BChar* b)
{
  return ::strncmp(a, b, la)==0 && b[la]==0;
}

template<>
inline const core::BChar* schr(const core::BChar* a, core::BChar ch)
{ 
  return ::strchr(a, ch); 
}

template<>
inline const core::BChar* sstr(const core::BChar* a, const core::BChar* ch)
{
  return ::strstr(a, ch);
}

} // namespace su
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_STRUTILS_CHAR_H
