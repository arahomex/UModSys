#ifndef __UMODSYS_LIBMEDIA_TYPEDEFS_H
#define __UMODSYS_LIBMEDIA_TYPEDEFS_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media common structures
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/tl/container/isockets.h>
#include <umodsys/tl/util/flags4state.h>


namespace UModSys {
namespace libmedia {

using namespace core;

//***************************************
// FORWARDS
//***************************************

// end-points:
struct IUtilities;
struct IStreamReader;            // read binary data
struct IStreamWriter;            // write binary data

struct ILibObject;
  struct IBinObjFilter;            // convert object<>data
  struct IBinArchive;              // data<>storage
    struct IBinArchiveFrame;
  struct ILibrary;                 // general library
    struct ILibraryBinTree;
    struct ILibraryBinCache;
    struct ILibraryObjFilter;
    struct ILibraryObjCache;
    struct ILibraryLayered;

struct ISerializeHash;
struct ISerializeArray;
struct ISerializeReader;
struct ISerializeWriter;

//***************************************

struct SMediaFlagUid;
struct SVFileName;
struct SVComplexFileName;
struct SFileInfo;
struct SFlags;
struct SFlagsChain;
struct SSerializeValue;
struct SBuffer;

//***************************************

template<typename Char> struct TTextStreamReader;
template<typename Char> struct TTextStreamWriter;
template<typename Char, size_t BufLength> struct TTextStreamReaderBuf;
template<typename Char, size_t BufLength> struct TTextStreamWriterBuf;


//***************************************
// TYPES
//***************************************

const size_t MaxFilenameSize = 0x1000;

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

enum eSerializeOptions {
  sero_Compact = 0x0001,
  sero_Strict  = 0x0002,
  sero_Expand  = 0x0004,
  sero_Naked   = 0x0010,
  sero_Default = 0x0000
};

struct SMediaFlagUid {};

//***************************************
//***************************************

typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufMedium, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DFixedFileName;

typedef Bsint64 DFilePosition;

typedef tl::TFlags4State<SMediaFlagUid, Buint32> DMediaFlags;

// binary operations
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  0> mf_archive;     // get binary from archives
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  1> mf_cache;       // get binary from cache
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  2> mf_failwrite;   // record binary failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  3> mf_failuse;     // use binary failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  4> mf_cachesave;   // update binary cache
// object operations
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  5> mf_filter;      // get object from filter
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  6> mf_objects;     // get object from object cache
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  7> mf_nullwrite;   // record object failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  8> mf_nulluse;     // use object failures
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32,  9> mf_objsave;     // update object cache
// miscelaneous operations
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32, 10> mf_safe;        // safe writes
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32, 11> mf_relpath;     // relative pathes
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32, 12> mf_relname;     // relative names
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32, 13> mf_groupnew;    // auto-create groups
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32, 14> mf_reserved1;   // R1
typedef struct tl::TFlags4StateShift<SMediaFlagUid, Buint32, 15> mf_reserved2;   // R2

typedef tl::TIStackSocket<SFileInfo> DIFileInfoArray;

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_TYPEDEFS_H
