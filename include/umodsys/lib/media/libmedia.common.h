#ifndef __UMODSYS_LIBMEDIA_COMMON_H
#define __UMODSYS_LIBMEDIA_COMMON_H 1
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

struct IDataLibrary;       // access to data by name (VFS)
struct IDataCache;         // cache data in memory
struct IDataArchive;       // data<>storage

struct IStreamReader;      // read binary data
struct IStreamWriter;      // write binary data

struct IObjectLibrary;     // access to object by name (VOS)
struct IObjectCache;       // cache objects in memory
struct IObjectFilter;      // convert object<>data

typedef tl::TStringBuf<
  tl::su::TSCorePrealloc<cStringBufMedium, core::BChar>, 
  tl::su::TComparerBinaryHash<core::BChar> 
> DFixedFileName;

typedef sint64 DFilePosition;

//***************************************
// TYPES
//***************************************

const size_t MaxFilenameSize = 0x1000;

/*
enum ePhase {
  mph_Null,
  mph_Parent,
  mph_System,
  mph_BinCache,
  mph_BinCacheUpdate,
  mph_BinArchive,
  mph_ObjCache,
  mph_ObjCacheUpdate,
  mph_ObjFilter,
  mph_Unknown
};
enum eFlagOptions {
  mfo_Default = 0,
  mfo_Yes     = 1,
  mfo_No      = 2,
  mfo_Parent  = 3,
  mfo_Mask    = 3
};
*/

//***************************************
//***************************************

struct SMediaFlagUid {};
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

struct SFileInfo {
  DStringShared name;
  BTime time;
  DFilePosition size;
  int permissions;
  //
  int entry_flags; // private attributes
  tl::TRefObject<IDataArchive> archive;
  DStringShared arch_name;
  int handle; // index or handle or something else

  inline SFileInfo(void) : handle(0), size(0), entry_flags(0), time(null_time) {}
  inline ~SFileInfo(void) {}
  inline SFileInfo(const SFileInfo& R) : handle(0), size(0), entry_flags(0), time(null_time) { *this = R; }
  //
  inline const SFileInfo& operator=(const SFileInfo& R) {
    archive = R.archive; name = R.name; arch_name = R.name;
    handle = R.handle;
    time = R.time; size = R.size;
    entry_flags = R.entry_flags;
    return *this;
  }
  //
  inline void clear(void) {
    archive.clear(); name.clear(); arch_name.clear();
    handle = 0; size = 0; entry_flags =0; time = null_time;
  }
};

typedef tl::TIStackSocket<SFileInfo> DIFileInfoArray;

//***************************************

struct SVFileName {
  BChar namebuf[MaxFilenameSize+1];
  DCString name;
  bool valid;
  int purenamelen;
  //
  inline const DCString& operator()(void) const { return name; }
  inline operator const DCString&(void) const { return name; }
  inline operator const BCStr(void) const { return name; }
  //
  bool relative_up(bool path, bool name);
  void init(BCStr whole, int wholelen, bool dir);
  //
  void operator=(BCStr filename);
  //
  SVFileName(BCStr whole="", bool dir=false);
  SVFileName(const DCString &whole, bool dir);
  SVFileName(const DCString &prefix, BCStr suffix, bool dir);
  SVFileName(const DCString &prefix, const DCString& suffix, bool dir);
};

struct SVComplexFileName {
  DFixedFileName filename;
  DCString upath, uname;
  int iterid;
  bool valid;
  //
  inline SVComplexFileName(void) : iterid(1), valid(false) {}
  //
  operator bool(void) const { return valid; }
  operator DCString(void) const { return filename(); }
  BCStr get_bs(void) const { return filename; }
  DCString operator*(void) const { return filename(); }
  //
  inline bool operator()(const DCString &up, const DCString &un) { return begin(up, un); }
  inline bool operator()(void) { return next(); }
  inline bool operator++(void) { return next(); }
  inline bool operator++(int) { return next(); }
  //
  bool begin(const DCString &up, const DCString &un);
  bool next(void);
};

//***************************************
// INTERFACES
//***************************************

struct SFlags {
  struct IResolver {
    virtual DMediaFlags::eStates get_parent_flag(int shift) const =0;
  };
  //
  DMediaFlags mf;
  const IResolver *ir;
  //
  inline SFlags(const DMediaFlags f=DMediaFlags(), const IResolver *r=NULL) : mf(f), ir(r) {}
  //
  inline const DMediaFlags& operator*(void) const { return mf; }
  //
  template<typename T> 
  bool is_flag(const DMediaFlags &auto_values, DMediaFlags::eStates desired) const { 
    DMediaFlags::eStates v = T::get(mf);
    if(v==DMediaFlags::Default) {
      if(ir!=NULL)
        return ir->get_parent_flag(T::base_shift)==desired;
      return T::get(auto_values)==desired;
    }
    return v==desired; 
  }
  template<typename T> bool yes(const DMediaFlags &auto_values) const { return is_flag<T>(auto_values, DMediaFlags::Yes); }
  template<typename T> bool no(void) const { return is_flag<T>(auto_values, DMediaFlags::No); }
};

//***************************************

struct IStreamReader 
: public IRefObject 
{
  virtual bool reader_seek(DFilePosition pos) =0;
  virtual bool reader_seekend(DFilePosition pos) =0;
  virtual bool reader_seekdelta(DFilePosition pos) =0;
  virtual DFilePosition reader_pos(void) =0;
  virtual DFilePosition reader_size(void) =0;
  virtual bool reader_read(void* data, size_t size) =0;
  virtual void reader_close(void) =0;
  //
  inline bool reader_read(const SMem& m) { return reader_read(m.data, m.size); }
  inline bool reader_read(const SMemChunk& m) { return reader_read(m.get_data(), m.get_size()); }
  inline bool reader_read(const SMemShared& m) { return reader_read(m.get_data(), m.get_size()); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IStreamReader, 2, IRefObject);
};

//***************************************

struct IStreamWriter 
: public IRefObject 
{
public:
  virtual bool writer_seek(DFilePosition pos) =0;
  virtual bool writer_seekend(DFilePosition pos) =0;
  virtual bool writer_seekdelta(DFilePosition pos) =0;
  virtual bool writer_setsize(DFilePosition pos) =0;
  virtual DFilePosition writer_pos(void) =0;
  virtual DFilePosition writer_size(void) =0;
  virtual bool writer_write(const void* data, size_t size) =0;
  virtual bool writer_copy(IStreamReader* from, DFilePosition count) =0;
  virtual bool writer_copy(IStreamReader* from, DFilePosition start, DFilePosition count) =0;
  virtual void writer_close(void) =0;
  virtual void writer_abort(void) =0;
  //
  inline bool writer_write(const SCMem& m) { return writer_write(m.data, m.size); }
  inline bool writer_write(const SMemChunk& m) { return writer_write(m.get_data(), m.get_size()); }
  inline bool writer_write(const SMemShared& m) { return writer_write(m.get_data(), m.get_size()); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IStreamWriter, 2, IRefObject);
};

//***************************************

/*
struct IDataLibrary
: public IRefObject 
{
public:
  virtual IMediaReader::P data_reader(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual IMediaWriter::P data_writer(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual bool data_load(SCMemShared& mem, const DCString& media_name, const SFlags& flags=SFlags()) =0; // load binary data
  virtual bool data_save(const SCMem& mem, const DCString& media_name, const SFlags& flags=SFlags()) =0; // save binary data
//  virtual bool file_info(SMediaFileInfo& info, const DHString& media_name, const SFlags& flags=SFlags()) =0; // determine attributes
//  virtual bool file_info(SMediaFileInfoArray& info, const DHString& media_mask, const SFlags& flags=SFlags()) =0; // determine attributes
  virtual bool archive_add(BCStr mountpoint, IMediaArchive* archive, int permit=mp_RL, int order=0) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IDataLibrary, 2, IRefObject);
};

struct IDataCache
: public IRefObject 
{
public:
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IDataLibrary, 2, IRefObject);
};
*/

//***************************************

struct IDataArchive 
: public IRefObject 
{
public:
  virtual IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags=SFlags()) =0;
  virtual bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags=SFlags()) =0;
  virtual bool data_list(const DCString &mask, DIFileInfoArray& list) = 0;
  virtual int get_permissions(void) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IDataArchive, 2, IRefObject);
};

//***************************************

#if 0
struct IObjectFilter 
: public IRefObject 
{
public:
  struct SRegistry {
    const SParameters& params; // filter parameters
    const IData
  };
  struct SInfo {
    SRegistry &reg;
//    IMediaGroup* group;
    UMS_CSMem binary;
    DCString media_name;
    DCString type_target;
    DCString common_type;
    const SParameters* params;
    int flags;
    //
/*
    inline SInfo(IMediaGroup *g, const UMS_CSMem& b, const SParameters* p,
                 DHString mn, DHString tn, int f=mf_Default)
    : group(g), binary(b), media_name(mn), type_target(tn), params(p), flags(f), common_type(NULL) 
    {}
    //
    inline SMediaFilter(IMediaGroup *g, const SParameters* p,
                        DHString mn, DHString tn, 
                        int f=mf_Default)
    : group(g), media_name(mn), type_target(tn), params(p), flags(f), common_type(NULL) 
    {}
    //
    inline ~SMediaFilter(void) {}
*/
  };
public:
  virtual bool load(IRefObject::P& obj, const SInfo& info) =0;
  virtual bool load(IRefObject* obj, const SInfo& info) =0;
  virtual bool save(IRefObject* obj, SInfo& info) =0;
//  virtual bool set_paramb(const DHString &kind, const DHString &name, const SCMem& memory) = 0;
//  virtual bool set_params(const DHString &kind, const SParameters* filter_params) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IObjectFilter, 2, IRefObject);
};
#endif

//***************************************
// core media utilities
//***************************************

/*
inline bool is_extension_case(const DCString& name, const DCString& ext) 
{
  if(~name<~ext)
    return false;
  if(tl::su::sncmp(name.text+~name-~ext, ext.text, ~ext)==0)
    return true;
  return false;
}

inline bool is_extension_nocase(const DLString& name, const DCString& ext) 
{
  if(~name<~ext)
    return false;
  if(tl::su::snicmp(name.text+~name-~ext, ext.text, ~ext)==0)
    return true;
  return false;
}
*/

//***************************************
// INLINES/OUTLINES
//***************************************

bool path_name(BCStr pathname, BCStr &name);
bool path_name(BCStr path, BCStr &name, size_t drops);
bool path_unite(BWStr pathname, size_t pathnamesize, BCStr path, BCStr name, bool purename=false);
bool path_replacename(BWStr pathname, size_t pathnamesize, BCStr pathname2, BCStr name, bool purename=false);
bool path_replacenamepath(BWStr pathname, size_t pathnamesize, BCStr pathname2, BCStr name, size_t drops, bool purename);

template<typename Buf> inline bool path_unite(Buf& buf, BStr path, BStr name, bool purename=false) 
{ 
  return path_unite(buf.get_buftext(), buf.get_bufmax(), path, name, purename); 
}
template<typename Buf> inline bool path_replacename(Buf& buf, BCStr path, BCStr name, bool purename=false) 
{ 
  return path_replacename(buf.get_buftext(), buf.get_bufmax(), path, name, purename); 
}
template<typename Buf> inline bool path_name(const Buf& buf, BCStr &name)
{ 
  return path_name(buf.get_buftext(), buf.get_bufmax(), name); 
}
template<typename Buf> inline bool path_replacenamepath(Buf& buf, BCStr path, BCStr name, size_t lostcount, bool purename=false) 
{ 
  return path_replacenamepath(buf.get_buftext(), buf.get_bufmax(), path, name, lostcount, purename); 
}

inline bool SVComplexFileName::begin(const DCString &up, const DCString &un) 
{
  upath = up; uname = un; iterid = 1;
  valid = path_replacename(filename, upath, uname, false); 
  return valid;
}

inline bool SVComplexFileName::next(void) 
{
  valid = path_replacenamepath(filename, upath, uname, ++iterid, false);
  return valid;
}

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_COMMON_H
