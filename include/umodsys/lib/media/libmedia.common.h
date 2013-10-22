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
  tl::TRefObject<IBinArchive> archive;
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
// INTERFACES
//***************************************

//***************************************
// IStreamReader::

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
// IStreamWriter::

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
// IDataArchive::

struct IBinArchive 
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
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinArchive, 2, IRefObject);
};

//***************************************
// IObjectFilter::

struct IBinObjFilter 
: public IRefObject 
{
public:
//  struct SRegistry {
//    const SParameters& params; // filter parameters
//  };
  struct SInfo {
//    SRegistry &reg;
//    IMediaGroup* group;
    SCMemShared binary;
    DCString media_name;
    DCString common_type;
    const SParameters* params;
    SFlags flags;
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
  virtual bool filter_load(IRefObject::P& obj, const SInfo& info) =0;
  virtual bool filter_load(IRefObject* obj, const SInfo& info) =0;
  virtual bool filter_save(IRefObject* obj, SInfo& info) =0;
//  virtual bool set_paramb(const DHString &kind, const DHString &name, const SCMem& memory) = 0;
//  virtual bool set_params(const DHString &kind, const SParameters* filter_params) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinObjFilter, 2, IRefObject);
};


//***************************************
// ILibrary:

struct ILibrary
: public IRefObject,
  public SFlags::IResolver
{
public:
  struct SObjOptions : public SFlags {
    DCString typehint;
    const SParameters* params;
    //
    inline SObjOptions(
      const DMediaFlags f=DMediaFlags(), const IResolver *r=NULL,
      const SParameters* p=NULL, const DCString& th=NULL
    ) : SFlags(f, r), typehint(th), params(p) {}
  };
public:
  // general data functions
  virtual IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags) =0;
  virtual IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags) =0;
  virtual bool bin_load(SCMemShared& mem, const DCString& media_name, const SFlags& flags) =0; // load binary data
  virtual bool bin_save(const SCMem& mem, const DCString& media_name, const SFlags& flags) =0; // save binary data
//  virtual bool bin_info(SMediaFileInfo& info, const DCString& media_name, const SFlags& flags) =0; // determine attributes
//  virtual bool bin_info(SMediaFileInfoArray& info, const DHString& media_mask, const SFlags& flags) =0; // determine attributes
  virtual bool bin_get(SCMemShared& mem, const DCString& media_name) =0; // get cache element
  virtual bool bin_put(const SCMemShared& mem, const DCString& media_name) =0; // put cache element
  // general object functions
  virtual bool obj_fget(IRefObject::P& obj, const IBinObjFilter::SInfo& info) =0;
  virtual bool obj_fload(IRefObject* obj, const IBinObjFilter::SInfo& info) =0;
  virtual bool obj_fsave(IRefObject* obj, IBinObjFilter::SInfo& info) =0;
  virtual bool obj_cget(IRefObject::P& obj, const DCString& media_name) =0; // get cache element
  virtual bool obj_cput(IRefObject* obj, const DCString& media_name) =0; // put cache element
  // universal object functions
  virtual bool obj_get(IRefObject::P& obj, const DCString& media_name, const SObjOptions& opts) =0;
  virtual bool obj_load(IRefObject* obj, const DCString& media_name, const SObjOptions& opts) =0;
  virtual bool obj_save(IRefObject* obj, const DCString& media_name, const SObjOptions& opts) =0;
public:
  inline bool obj_get(IRefObject::P& obj, const DCString& media_name, 
                      const SParameters* params = NULL, 
                      const DMediaFlags& flags = DMediaFlags(), 
                      const DCString& typehint = NULL) {
    return obj_get(obj, media_name, SObjOptions(flags, this, params, typehint));
  }
  inline bool obj_load(IRefObject* obj, const DCString& media_name, 
                       const SParameters* params = NULL, 
                       const DMediaFlags& flags = DMediaFlags(), 
                       const DCString& typehint = NULL) {
    return obj_load(obj, media_name, SObjOptions(flags, this, params, typehint));
  }                      
  inline bool obj_save(IRefObject* obj, const DCString& media_name, 
                       const SParameters* params = NULL, 
                       const DMediaFlags& flags = DMediaFlags(), 
                       const DCString& typehint = NULL) {
    return obj_save(obj, media_name, SObjOptions(flags, this, params, typehint));
  }
  //
  inline IStreamReader::P bin_reader(const DCString& media_name, const DMediaFlags& flags = DMediaFlags()) {
    return bin_reader(media_name, SFlags(flags, this));
  }
  inline IStreamWriter::P bin_writer(const DCString& media_name, const DMediaFlags& flags = DMediaFlags()) {
    return bin_writer(media_name, SFlags(flags, this));
  }
  inline bool bin_load(SCMemShared& mem, const DCString& media_name, const DMediaFlags& flags = DMediaFlags()) {
    return bin_load(mem, media_name, SFlags(flags, this));
  }
  inline bool bin_save(const SCMemShared& mem, const DCString& media_name, const DMediaFlags& flags = DMediaFlags()) {
    return bin_save(mem, media_name, SFlags(flags, this));
  }
protected:
  // this is std (non-array) functionality
  bool obj_std_get(IRefObject::P& obj, const DCString& media_name, const SObjOptions& opts);
  bool obj_std_load(IRefObject* obj, const DCString& media_name, const SObjOptions& opts);
  bool obj_std_save(IRefObject* obj, const DCString& media_name, const SObjOptions& opts);
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibrary, 2, IRefObject);
};

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
