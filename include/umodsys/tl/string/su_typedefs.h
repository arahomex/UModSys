#ifndef __UMODSYS_TYPES_STRING_UTILS_TYPEDEFS_H
#define __UMODSYS_TYPES_STRING_UTILS_TYPEDEFS_H 1
/*************************************************************/
// file: umodsys/tl/string/su_typedefs.h
// info: string utilities and strings typedefs
/*************************************************************/

#include <umodsys/common/arch.h>
#include <umodsys/common/mem/mem_alloc.h>

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

template<typename CharT, typename MemAllocT> 
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

// implemented in "string_const.h"
template<typename CoreT=su::TSCoreConst<core::BChar>, 
         typename Comparer=su::TComparerBinaryHash<typename CoreT::Char> > 
struct TString;     // const string

// implemented in "string_buffer.h"
template<typename CoreT=su::TSCoreAlloc<core::BChar>, 
         typename Comparer=su::TComparerBinaryHash<typename CoreT::Char> >
struct TStringBuf;    // string buffer

/*
template<size_t MaxLen, typename CharT=core::BChar>
struct TPrintBuffer;
*/

/*
// implemented in "string_list.h"
template<typename CharT=core::BChar>
struct TListStr;
template<typename TString=TString<> >
struct TListString;
*/

// implemented in "string_array.h"
template<size_t N, typename CharT=core::BChar>
struct TArrayStr; // array of const [char] pointers

// implemented in "string_array.h"
//template<size_t N, typename TString=TString<> >
//struct TArrayString; // array of [string]


// implemented in "string_pool.h"
template<typename CharT = core::BChar, 
         typename Comparer=su::TComparerBinaryHash<CharT>, 
         typename MemAllocT=core::SMemAlloc_Malloc >
struct TStaticStringPool; // static pool of string data


/*
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


} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_TYPEDEFS_H
