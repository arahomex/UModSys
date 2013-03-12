#ifndef __UMODSYS_TYPES_STRING_UTILS_SHARED_H
#define __UMODSYS_TYPES_STRING_UTILS_SHARED_H 1
//*****************************************************
// umodsys/types/su_base.h: string utilities base
//*****************************************************

#include <umodsys/core/arch.h>
#include <umodsys/core/allocator.h>

//#define S_STRING_COLLATE_CON const ICharCollate &col=su::collate()


namespace UModSys {
namespace tl {
namespace su {
//***************************************
// typedefs


using core::BChar;
using core::BCharW;
using core::BCharL;

template<typename CharT> 
struct TCharOptions;
template<typename CharT> 
struct TComparerBinaryHash;
template<typename CharT> 
struct TComparerBinary;

template<typename CharT> 
struct TSharedStringInfo;

template<typename CharT=core::BChar> 
struct TSCoreConst;     // string core - const (read)

template<typename CharT=core::BChar> 
struct TSCoreBuffer;    // string core - buffer (write)

template<size_t nChars, 
         typename CharT=core::BChar > 
struct TSCorePrealloc;  // string core - preallocated (read/write)

template<typename MemAllocT=core::SMemAlloc_Malloc, 
         typename CharT=core::BChar>
struct TSCoreShared;    // string core - shared (read)

template<typename MemAllocT=core::SMemAlloc_Malloc, 
         typename CharT=core::BChar>
struct TSCoreSharedBuf; // string core - shared (write)

template<typename MemAllocT=core::SMemAlloc_Malloc, 
         typename CharT=core::BChar>
struct TSCoreAlloc;     // string core - allocated (read)

template<typename MemAllocT=core::SMemAlloc_Malloc, 
         typename CharT=core::BChar>
struct TSCoreAllocBuf;  // string core - allocated (write)

//***************************************
// end

} // namespace su

//***************************************
//***************************************
//***************************************

//***************************************
// typedefs

template<typename CoreT=su::TSCoreConst<core::BChar>, 
         typename Comparer=su::TComparerBinaryHash<typename CoreT::Char> > 
struct TString;     // const string

template<typename CoreT=su::TSCoreAlloc<core::BChar>, 
         typename Comparer=su::TComparerBinaryHash<typename CoreT::Char> >
struct TStringBuf;    // string buffer

/*
template<size_t MaxLen, typename CharT=core::BChar>
struct TPrintBuffer;
*/
/*
template<typename CharT=core::BChar>
struct TListStr;
template<typename TString=TString<> >
struct TListString;

template<size_t N, typename CharT=core::BChar>
struct TArrayStr;
template<size_t N, typename TString=TString<> >
struct TArrayString;

typedef TString<core::BChar>            DString;
typedef TListStr<core::BChar>           DListStr;
*/

/*
typedef TLHAString<core::BChar>           DAString, DLHAString;
typedef TLHSString<core::BChar>           DSString, DLHSString;

typedef TPrintBuffer<256, core::BChar>    DPrintBufShort;
typedef TPrintBuffer<4096, core::BChar>   DPrintBufLong;
*/

//***************************************
// end


//***************************************
//***************************************
//***************************************

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

//***************************************
// end

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_SHARED_H
