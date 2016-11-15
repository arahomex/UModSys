#ifndef __UMODSYS_COMMON_STDSTRINGS_H
#define __UMODSYS_COMMON_STDSTRINGS_H 1
/*************************************************************/
// file: umodsys/common/stdstrings.h
// info: std string typedefs
/*************************************************************/

#include <umodsys/tl/string/strings.h>

namespace UModSys {
namespace core {

//***************************************
// Strings
//***************************************

const size_t cStringBufSmall   = 0x000100; // 256
const size_t cStringBufMedium  = 0x001000; // 4k
const size_t cStringBufLarge   = 0x010000; // 64k
const size_t cStringBufHuge    = 0x100000; // 1M

typedef tl::TString<
  tl::su::TSCoreConst<core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DCString;
typedef tl::TString<
  tl::su::TSCoreConst<core::BChar>, 
  tl::su::TComparerUtfNocaseHash<core::BChar> 
> DCNocaseString;

/*
typedef tl::TString<
  tl::su::TSCoreBuffer<core::BChar>, 
  tl::su::TComparerUtfNocaseHash<core::BChar> 
> DStringBuffer;
typedef DStringBuffer::BufferStr DStringBufferStr;
*/

typedef tl::TStringBuf<
  tl::su::TSCoreBuffer<core::BChar>, 
  tl::su::TComparerUtfNocaseHash<core::BChar> 
> DStringBuffer;

typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufSmall, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringBufSmall;
typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufMedium, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringBufMedium;
typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufLarge, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringBufLarge;
typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufHuge, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringBufHuge;


typedef tl::TString<
  tl::su::TSCoreShared<SIMemAlloc, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringShared;
typedef tl::TStringBuf<
  tl::su::TSCoreShared<SIMemAlloc, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringBufShared;

typedef tl::TString<
  tl::su::TSCoreShared<SMemAlloc_Malloc, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DStringSharedMalloc;

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_COMMON_STDSTRINGS_H
