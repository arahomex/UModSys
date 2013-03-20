#ifndef __UMS_CORE_MEDIA_MEDIA_H
#define __UMS_CORE_MEDIA_MEDIA_H 1
//*****************************************************
// ums/core/media/media.h: media library interface
//*****************************************************

#include <ums/core/system/system.h>

namespace UMS {
namespace core {
namespace media {

//***************************************
//***************************************

typedef sint32 FilePosition;


enum eMediaPhase {
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

enum eMediaFlagOptions {
  mfo_Default = 0,
  mfo_Yes     = 1,
  mfo_No      = 2,
  mfo_Parent  = 3,
  mfo_Mask    = 3
};

//***************************************
//***************************************

template<int base_shift> struct TMediaFlagOption {
  enum {
    shift = base_shift<<1,
    Yes = mfo_Yes << shift,
    No = mfo_No << shift,
    Default = mfo_Default << shift,
    Parent = mfo_Parent << shift
  };
  inline static eMediaFlagOptions get(int flags) { return eMediaFlagOptions((flags>>shift)&mfo_Mask); }
  inline static eMediaFlagOptions gets(int flags, int shift) { return eMediaFlagOptions((flags>>shift)&mfo_Mask); }
  inline static eMediaFlagOptions getS(int flags, int shift) { return eMediaFlagOptions((flags>>(shift<<1))&mfo_Mask); }
};

// binary operations
typedef struct TMediaFlagOption< 0> mf_archive;     // get binary from archives
typedef struct TMediaFlagOption< 1> mf_cache;       // get binary from cache
typedef struct TMediaFlagOption< 2> mf_failwrite;   // record binary failures
typedef struct TMediaFlagOption< 3> mf_failuse;     // use binary failures
typedef struct TMediaFlagOption< 4> mf_cachesave;   // update binary cache
// object operations
typedef struct TMediaFlagOption< 5> mf_filter;      // get object from filter
typedef struct TMediaFlagOption< 6> mf_objects;     // get object from object cache
typedef struct TMediaFlagOption< 7> mf_nullwrite;   // record object failures
typedef struct TMediaFlagOption< 8> mf_nulluse;     // use object failures
typedef struct TMediaFlagOption< 9> mf_objsave;     // update object cache
// miscelaneous operations
typedef struct TMediaFlagOption<10> mf_safe;        // safe writes
typedef struct TMediaFlagOption<11> mf_relpath;     // relative pathes
typedef struct TMediaFlagOption<12> mf_relname;     // relative names
typedef struct TMediaFlagOption<13> mf_groupnew;    // auto-create groups
typedef struct TMediaFlagOption<14> mf_reserved1;   // R1
typedef struct TMediaFlagOption<15> mf_reserved2;   // R2

enum eMediaFlags {
  mf_Default          = 0
/*
  mf_NoArchiveData    = 0x00000100,     // don't use archives
  mf_NoFilterData     = 0x00000200,     // don't use filters
  mf_NoRecordData     = 0x00000400,     // don't save binary data
  mf_NoRecordObject   = 0x00000800,     // don't save objects
  mf_CachedData       = mf_NoArchiveData | mf_NoFilterData,
  mf_RecordFailed     = 0x00001000,     // record failed attempts
  mf_RequestFailed    = 0x00002000,     // request failed attempts
  mf_NoCreateGroup    = 0x00004000,     // don't auto-create group
  mf_RelativePath     = 0x00010000,     // use relative pathes 
  mf_RelativeName     = 0x00020000,     // use relative names
  mf_Relative         = mf_RelativePath | mf_RelativeName,
  mf_SafeFileWrite    = 0x01000000,     // safe file operations (transactions)
*/
};

enum eMediaPermissions {
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

struct SMediaFileInfo {
  TRefObject<IMediaArchive> archive;
  int handle; // index or handle or something else
  DAString name;
  UMS_Time time;
  sint64 size;
  int entry_flags; // private attributes

  inline SMediaFileInfo(void)
    : handle(0), size(0), entry_flags(0), time(null_time) {}
  inline ~SMediaFileInfo(void) {
    name.free();
  }
  inline SMediaFileInfo(const SMediaFileInfo& R)
    : handle(0), size(0), entry_flags(0), time(null_time) {
      *this = R;
  }
  inline const SMediaFileInfo& operator=(const SMediaFileInfo& R) {
    name.alloc(R.name);
    handle = R.handle;
    time = R.time;
    size = R.size;
    entry_flags = R.entry_flags;
    archive = R.archive;
    return *this;
  }
  //
  inline void free(void) {
    name.free();
    archive.clear();
    handle = 0; size = 0;
    entry_flags =0; time = null_time;
  }
  //
  inline static void free_list(SMediaFileInfo* list, int count) {
    if(list && count) {
      for(int i=0; i<count; i++, list++) {
        list->free();
      }
    }
  }
};

struct SMediaFileInfoArray {
  SMediaFileInfo* list;
  int count, max_count;
  //
  inline void clear(void) {
    SMediaFileInfo::free_list(list, count);
    count = 0;
  }
  //
  int operator~(void) const { return count; }
  const SMediaFileInfo& operator[](int id) const { return list[id]; }
  SMediaFileInfo& operator[](int id) { return list[id]; }
  //
  inline SMediaFileInfoArray(SMediaFileInfo& L)
    : count(0), max_count(1), list(&L) {}
  inline SMediaFileInfoArray(SMediaFileInfo* L, int num)
    : count(0), max_count(num), list(L) {}
  inline ~SMediaFileInfoArray(void) {
    clear();
  }
};

template<int FixedCount>
struct TMediaFileInfoArray : public SMediaFileInfoArray {
  enum {
    MaxCount = FixedCount
  };
  SMediaFileInfo fixed_list[MaxCount];
  //
  inline TMediaFileInfoArray(void) 
    : SMediaFileInfoArray(fixed_list, MaxCount) {}
  inline ~TMediaFileInfoArray(void) {}
};

//***************************************

struct SVFileName {
  BChar namebuf[UMS_OS_FILENAME_SIZE+1];
  DHString name;
  bool valid;
  int purenamelen;
  //
  inline const DHString& operator()(void) const { return name; }
  inline operator const DHString&(void) const { return name; }
  inline operator const BCStr(void) const { return name; }
  //
  bool relative_up(bool path, bool name);
  void init(BCStr whole, int wholelen, bool dir);
  //
  void operator=(BCStr filename);
  //
  SVFileName(BCStr whole="", bool dir=false);
  SVFileName(const DHString &whole, bool dir=false);
  SVFileName(const DHString &prefix, BCStr suffix);
  SVFileName(const DHString &prefix, const DHString& suffix);
};

struct SVComplexFileName {
  TPrintBuffer<UMS_OS_FILENAME_SIZE+1> filename;
  DHString upath, uname;
  int iterid;
  bool valid;
  //
  inline SVComplexFileName(void) : iterid(1), valid(false) {}
  //
  operator bool(void) const { return valid; }
  operator DHString(void) const { return filename; }
  BCStr get_bs(void) const { return filename; }
  DHString operator*(void) const { return filename; }
  //
  inline bool operator()(const DHString &up, const DHString &un) { return begin(up, un); }
  inline bool operator()(void) { return next(); }
  inline bool operator++(void) { return next(); }
  inline bool operator++(int) { return next(); }
  //
  inline bool begin(const DHString &up, const DHString &un) {
    upath = up; uname = un; iterid = 1;
    valid = filename.path_replacename(upath, uname, false); 
    return valid;
  }
  inline bool next(void) {
    valid = filename.path_replacenamepath(upath, uname, ++iterid, false);
    return valid;
  }
};

//***************************************

struct IMediaReader : public IRefObject {
  virtual bool reader_seek(FilePosition pos) =0;
  virtual bool reader_seekend(FilePosition pos) =0;
  virtual bool reader_seekdelta(FilePosition pos) =0;
  virtual FilePosition reader_pos(void) =0;
  virtual FilePosition reader_size(void) =0;
  virtual bool reader_read(void* data, size_t size) =0;
  virtual void reader_close(void) =0;
  //
  inline bool reader_read(const SMem& m) { return reader_read(m.data, m.size); }
  inline bool reader_read(const SMemChunk& m) { return reader_read(m.get_data(), m.get_size()); }
  inline bool reader_read(const SMemShared& m) { return reader_read(m.get_data(), m.get_size()); }
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::IMediaReader, IRefObject)
};

//***************************************

struct IMediaWriter : public IRefObject {
  virtual bool writer_seek(FilePosition pos) =0;
  virtual bool writer_seekend(FilePosition pos) =0;
  virtual bool writer_seekdelta(FilePosition pos) =0;
  virtual bool writer_setsize(FilePosition pos) =0;
  virtual FilePosition writer_pos(void) =0;
  virtual FilePosition writer_size(void) =0;
  virtual bool writer_write(const void* data, size_t size) =0;
  virtual bool writer_copy(IMediaReader* from, FilePosition count) =0;
  virtual bool writer_copy(IMediaReader* from, FilePosition start, FilePosition count) =0;
  virtual void writer_close(void) =0;
  virtual void writer_abort(void) =0;
  //
  inline bool writer_write(const SCMem& m) { return writer_write(m.data, m.size); }
  inline bool writer_write(const SMemChunk& m) { return writer_write(m.get_data(), m.get_size()); }
  inline bool writer_write(const SMemShared& m) { return writer_write(m.get_data(), m.get_size()); }
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::IMediaWriter, IRefObject)
};

//***************************************

struct IMediaGroup : public IRefObject {
  virtual IMediaLibrary* get_lib(void) =0;
  virtual IMediaGroup* get_root(void) =0; // root group which holds media binaries & archives
  virtual IMediaGroup* get_parent(void) =0; // parent group
  virtual BCStr get_name(void) =0;
  virtual IMediaGroup* get_group(const DHString& group_name, bool autocreate=false) =0; // sub-groups
  virtual eMediaPhase get_last_phase(void) =0; // return last operation last phase
  virtual int get_flags(void) =0; // return default flags
  virtual void set_flags(int flags) =0; // return default flags
  virtual void* get_cache_data(size_t size) =0; // cache data private to this group
  //
  virtual IMediaReader::P load_reader(const DHString& media_name, int flags=mf_Default) =0;
  virtual IMediaWriter::P save_writer(const DHString& media_name, int flags=mf_Default) =0;
  virtual bool load_data(UMS_CSMem& mem, const DHString& media_name, int flags=mf_Default) =0; // load binary data
  virtual bool save_data(const SCMem& mem, const DHString& media_name, int flags=mf_Default) =0; // save binary data
  virtual bool file_info(SMediaFileInfo& info, const DHString& media_name, int flags=mf_Default) =0; // determine attributes
  virtual bool file_info(SMediaFileInfoArray& info, const DHString& media_mask, int flags=mf_Default) =0; // determine attributes
  virtual bool get_object(IRefObject::P& obj,
                          const DHString& group, const DHString& media_name, 
                          const SParameters* params, int flags=mf_Default) =0;
  virtual bool load_object(IRefObject* obj, const DHString& group, 
                           const DHString& media_name, const SParameters* params, 
                           int flags=mf_Default, const DHString& typehint=NULL) =0;
  virtual bool save_object(IRefObject* obj, const DHString& group, 
                           const DHString& media_name, const SParameters* params, 
                           int flags=mf_Default, const DHString& typehint=NULL) =0;
  virtual bool filter_data_load(IRefObject::P& obj, const UMS_CSMem& mem, const DHString& group, 
                                const DHString& media_name, const SParameters* params) =0;
  virtual bool filter_data_load(IRefObject::P& obj, const SMediaFilter& info) =0;
  virtual bool filter_data_load(IRefObject* obj, const SMediaFilter& info) =0;
  virtual bool filter_data_save(IRefObject* obj, SMediaFilter& info) =0;
  //
  virtual bool add_object(IRefObject* obj, const DHString& media_name, const DHString& group) =0;
  virtual bool add_filter(IMediaFilter* filter, const SParameters* filter_params=NULL, int order=0) =0;
  virtual bool add_archive(BCStr mountpoint, IMediaArchive* archive, int permit=mp_RL, int order=0) =0;
  //
  virtual bool remove_object(IRefObject* obj) =0;
  virtual bool remove_filter(IMediaFilter* filter, int order=0) =0;
  virtual bool remove_archive(BCStr mountpoint, int order=0) =0;
  virtual bool remove_group(IMediaGroup* group) =0;
  //
  virtual bool clear_bins(void) =0; // clear binaries
  virtual bool clear_objects(void) =0; // clear objects
  virtual bool clear_filters(void) =0; // clear filters
  virtual bool clear_archives(void) =0; // clear archivers
  virtual bool clear_groups(void) =0; // clear sub-groups
  virtual bool cleanup_groups(void) =0; // remove all unused groups
  //
  template<typename RData>
  inline bool t_get_object(TRefObject<RData> &rv, const DHString& group, const DHString& name, 
                           const SParameters* params = NULL, int flags=mf_Default) {
    rv = NULL;
    IRefObject::P v;
    if(!get_object(v, group, name, params, flags)) return false;
    rv = static_cast<RData*>( v->get_other_interface(RData::_get_interface_type()) );
    return rv.valid();
  }
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::IMediaGroup, IRefObject)
};

//***************************************

struct SMediaFilter {
  IMediaGroup* group;
  UMS_CSMem binary;
  DHString media_name;
  DHString type_target;
  DHString common_type;
  const SParameters* params;
  int flags;
  //
  inline SMediaFilter(IMediaGroup *g, const UMS_CSMem& b, const SParameters* p,
                      DHString mn, DHString tn, 
                      int f=mf_Default)
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
};

struct IMediaFilter : public IRefObject {
  virtual bool load(IRefObject::P& obj, const SMediaFilter& info, const SParameters& filter_params) =0;
  virtual bool load(IRefObject* obj, const SMediaFilter& info, const SParameters& filter_params) =0;
  virtual bool save(IRefObject* obj, SMediaFilter& info, const SParameters& filter_params) =0;
  virtual bool set_paramb(const DHString &kind, const DHString &name, const SCMem& memory) = 0;
  virtual bool set_params(const DHString &kind, const SParameters* filter_params) = 0;
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::IMediaFilter, IRefObject)
};

//***************************************

struct IMediaArchive : public IRefObject {
  virtual bool load_data(IMediaGroup* glib, UMS_CSMem& mem, const DHString& media_name, int flags=mf_Default) =0;
  virtual IMediaReader::P load_reader(IMediaGroup* glib, const DHString& media_name, int flags=mf_Default) =0;
  virtual bool save_data(IMediaGroup* glib, const SCMem& mem, const DHString& media_name, int flags=mf_Default) =0;
  virtual IMediaWriter::P save_writer(IMediaGroup* glib, const DHString& media_name, int flags=mf_Default) =0;
  virtual bool get_entrylist(const DHString &mask, SMediaFileInfoArray& list) = 0;
  virtual int get_permissions(void) = 0;
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::IMediaArchive, IRefObject)
};

//***************************************

struct IMediaLibrary : public ISysObject {
  virtual int get_flags(void) =0; // return default flags
  virtual void set_flags(int flags) =0; // return default flags
  virtual IMediaGroup* get_group(const DHString& group_name, bool autocreate=false) =0; // root groups only
  virtual IMediaGroup* get_group(const DHString* group_names, bool autocreate=false) =0; // any groups
  virtual bool remove_group(IMediaGroup* group) =0;
  virtual void clear_groups(void) =0; // remove all groups
  virtual void cleanup_groups(void) =0; // remove all unused groups
  //
  virtual IMediaArchive::P new_archive_osdir(BCStr osname, int permissions) =0;
  virtual IMediaArchive::P new_archive_frame(IMediaArchiveFrameClient* cl, const SParameters* params) = 0;
  virtual IMediaReader::P new_reader_mem(const SCMem& mem) =0;
  virtual IMediaWriter::P new_writer_mem(const SMem& mem) =0;
  virtual IMediaReader::P new_reader_smem(const UMS_CSMem& mem) =0;
  virtual IMediaWriter::P new_writer_smem(const SMemShared& mem) =0;
  virtual IMediaReader::P new_reader_osfile(BCStr osname) =0;
  virtual IMediaWriter::P new_writer_osfile(BCStr osname, int flags) =0;
  virtual IMediaReader::P new_reader_sub(IMediaReader* f, FilePosition start, FilePosition size) =0;
  virtual IMediaWriter::P new_writer_sub(IMediaReader* f, FilePosition start, FilePosition size) =0;
protected:
};

//***************************************
// core media utilities
//***************************************

inline bool is_extension_case(const DLString& name, const DLString& ext) 
{
  if(~name<~ext)
    return false;
  if(su::sncmp(name.text+~name-~ext, ext.text, ~ext)==0)
    return true;
  return false;
}

inline bool is_extension_nocase(const DLString& name, const DLString& ext) 
{
  if(~name<~ext)
    return false;
  if(su::snicmp(name.text+~name-~ext, ext.text, ~ext)==0)
    return true;
  return false;
}

//***************************************

} // namespace media
} // namespace core
} // namespace UMS

#endif
