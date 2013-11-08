#ifndef __UMODSYS_LIBMEDIA_TYPEDEFS_H
#define __UMODSYS_LIBMEDIA_TYPEDEFS_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media common structures
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/tl/composite/isockets.h>
#include <umodsys/tl/util/flags4state.h>


namespace UModSys {
namespace libmedia {

using namespace core;

//***************************************
// FORWARDS
//***************************************

// end-points:
struct IStreamReader;            // read binary data
struct IStreamWriter;            // write binary data
struct IBinArchive;              // data<>storage
struct IBinObjFilter;            // convert object<>data

struct ILibrary;
  struct ILibraryBinTree;
  struct ILibraryBinCache;
  struct ILibraryObjFilter;
  struct ILibraryObjCache;
  struct ILibraryLayered;

typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufMedium, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DFixedFileName;

typedef sint64 DFilePosition;

//***************************************
// TYPES
//***************************************

const size_t MaxFilenameSize = 0x1000;
struct SMediaFlagUid {};

enum ePermissions {
  mp_Read             = 0x0001,
  mp_Write            = 0x0002,
  mp_List             = 0x0004,
  mp_Filter           = 0x0008,
  //
  mp_RW               = mp_Read | mp_Write,
  mp_RL               = mp_Read | mp_List,
  mp_WL               = mp_Write | mp_List,
  mp_RWL              = mp_Read | mp_Write | mp_List,
  //
  mp_All              = mp_Read | mp_Write | mp_List | mp_Filter
};

//***************************************
//***************************************

typedef tl::TFlags4State<SMediaFlagUid, uint32> DMediaFlags;

// binary operations
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  0> mf_archive;     // get binary from archives
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  1> mf_cache;       // get binary from cache
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  2> mf_failwrite;   // record binary failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  3> mf_failuse;     // use binary failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  4> mf_cachesave;   // update binary cache
// object operations
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  5> mf_filter;      // get object from filter
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  6> mf_objects;     // get object from object cache
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  7> mf_nullwrite;   // record object failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  8> mf_nulluse;     // use object failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32,  9> mf_objsave;     // update object cache
// miscelaneous operations
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32, 10> mf_safe;        // safe writes
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32, 11> mf_relpath;     // relative pathes
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32, 12> mf_relname;     // relative names
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32, 13> mf_groupnew;    // auto-create groups
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32, 14> mf_reserved1;   // R1
typedef struct tl::TFlags4StateShift<SMediaFlagUid, uint32, 15> mf_reserved2;   // R2

//***************************************

struct SFileInfo;
typedef tl::TIStackSocket<SFileInfo> DIFileInfoArray;

//***************************************

struct SVFileName;

struct SVComplexFileName;

//***************************************

struct SFlags;
struct SFlagsChain;

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_TYPEDEFS_H
