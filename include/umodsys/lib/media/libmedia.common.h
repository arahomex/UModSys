#ifndef __UMODSYS_LIBMEDIA_COMMON_H
#define __UMODSYS_LIBMEDIA_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media common structures
/*************************************************************/

#include <umodsys/lib/media/libmedia.typedefs.h>


namespace UModSys {
namespace libmedia {

//***************************************
// TYPES
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
    virtual DMediaFlags::eStates get_flag(int shift) const =0;
  };
  struct IResolverAuto : public IResolver {
    virtual DMediaFlags get_flags_auto(void) const =0;
  };
  struct ISetter : public IResolverAuto {
    virtual DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) =0;
  };
  //
  //
  DMediaFlags mf;
  const IResolver *ir;
  //
  inline SFlags(const DMediaFlags f=DMediaFlags(), const IResolver *r=NULL) : mf(f), ir(r) {}
  //
  inline const DMediaFlags& operator*(void) const { return mf; }
  //
  template<typename T> 
  inline bool is_flag(const IResolverAuto *crx, DMediaFlags::eStates desired) const { 
    DMediaFlags::eStates v = T::get(mf);
    if(v==DMediaFlags::Default) {
      if(crx!=NULL)
        v = crx->get_flag(T::base_shift);
      if(v==DMediaFlags::Default && ir!=NULL)
        v = ir->get_flag(T::base_shift);
      if(v==DMediaFlags::Default && crx!=NULL)
        v = T::get(crx->get_flags_auto());
    }
    return v==desired; 
  }
  template<typename T> 
  inline bool yes(const IResolverAuto *crx) const { 
    return is_flag<T>(crx, DMediaFlags::Yes); 
  }
  template<typename T> 
  inline bool no(const IResolverAuto *crx) const { 
    return is_flag<T>(crx, DMediaFlags::No); 
  }
};

struct SFlagsChain : public SFlags {
  struct RResolverChain : public IResolver {
    const IResolver *next;
    const IResolver *cur;
    //
    inline RResolverChain(const IResolver *c, const IResolver *n) : next(n), cur(c) {}
    //
    DMediaFlags::eStates get_flag(int shift) const {
      DMediaFlags::eStates rv = cur->get_flag(shift);
      if(rv!=DMediaFlags::Default)
        return rv;
      return next==NULL ? DMediaFlags::Auto : next->get_flag(shift);
    }
  };
  //
  //
  RResolverChain rc;
  //
  SFlagsChain(const SFlags& pf, const IResolver* r) : rc(r, pf.ir), SFlags(pf.mf, &rc) {}
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
: public IRefObject, 
  public SFlags::ISetter
{
public:
  virtual IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags=SFlags()) =0;
  virtual bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags=SFlags()) =0;
  virtual bool data_list(const DCString& mask, DIFileInfoArray& list, const SFlags& flags=SFlags()) = 0;
  virtual int get_permissions(void) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinArchive, 2, IRefObject);
};

//***************************************
// IObjectFilter::

struct IBinObjFilter 
: public IRefObject,
  public SFlags::ISetter
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
    TypeId reqtype;
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
  virtual bool filter_load(const SInfo& info, IRefObject::P& obj) =0;
  virtual bool filter_load(const SInfo& info, IRefObject* obj) =0;
  virtual bool filter_save(SInfo& info, const IRefObject* obj) =0;
//  virtual bool set_paramb(const DHString &kind, const DHString &name, const SCMem& memory) = 0;
//  virtual bool set_params(const DHString &kind, const SParameters* filter_params) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinObjFilter, 2, IRefObject);
};


//***************************************
// ILibrary:

struct ILibrary
: public IRefObject,
  public SFlags::ISetter
{
public:
  struct SObjOptions : public SFlags {
    DCString typehint;
    TypeId reqtype;
    const SParameters* params;
    //
    inline SObjOptions(
      TypeId rt = NULL,
      const DMediaFlags f=DMediaFlags(), const IResolver *r=NULL,
      const SParameters* p=NULL, const DCString& th=NULL
    ) : SFlags(f, r), reqtype(rt), typehint(th), params(p) {}
  };
public:
  // general data functions
  virtual IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags) =0;
  virtual IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags) =0;
  virtual bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) =0; // load binary data
  virtual bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags) =0; // save binary data
  virtual bool bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags) =0; // determine attributes
  virtual bool bin_info(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags) =0; // get full list
  virtual bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv=false) =0; // get cache element
  virtual bool bin_put(const DCString& media_name, const SCMemShared* mem) =0; // put cache element
  // general object functions
  virtual bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj) =0;
  virtual bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj) =0;
  virtual bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj) =0;
  virtual bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv=false) =0; // get cache element
  virtual bool obj_cput(const DCString& media_name, IRefObject* obj) =0; // put cache element
  // universal object functions
  virtual bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts) =0;
  virtual bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts) =0;
  virtual bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts) =0;
public:
  inline IStreamReader::P bin_reader(const DCString& media_name, const DMediaFlags& flags = DMediaFlags()) {
    return bin_reader(media_name, SFlags(flags, this));
  }
  inline IStreamWriter::P bin_writer(const DCString& media_name, const DMediaFlags& flags = DMediaFlags()) {
    return bin_writer(media_name, SFlags(flags, this));
  }
  inline bool bin_load(const DCString& media_name, SCMemShared& mem, const DMediaFlags& flags = DMediaFlags()) {
    return bin_load(media_name, mem, SFlags(flags, this));
  }
  inline bool bin_save(const DCString& media_name, const SCMemShared& mem, const DMediaFlags& flags = DMediaFlags()) {
    return bin_save(media_name, mem, SFlags(flags, this));
  }
  //
  inline bool obj_load(const DCString& media_name, IRefObject* obj,
                       const SParameters* params = NULL, 
                       const DMediaFlags& flags = DMediaFlags(), 
                       const DCString& typehint = NULL) {
    return obj_load(media_name, obj, SObjOptions(NULL, flags, this, params, typehint));
  }                      
  inline bool obj_save(const DCString& media_name, const IRefObject* obj, 
                       const SParameters* params = NULL, 
                       const DMediaFlags& flags = DMediaFlags(), 
                       const DCString& typehint = NULL) {
    return obj_save(media_name, obj, SObjOptions(NULL, flags, this, params, typehint));
  }
  inline bool obj_get(const DCString& media_name, IRefObject::P& obj, 
                      const SParameters* params = NULL, 
                      const DMediaFlags& flags = DMediaFlags(), 
                      const DCString& typehint = NULL) {
    return obj_get(media_name, obj, SObjOptions(NULL, flags, this, params, typehint));
  }
  template<typename T>
  inline bool t_obj_get(const DCString& media_name, tl::TRefObject<T>& obj, 
                        const DMediaFlags f=DMediaFlags(), const SParameters* par=NULL,
                        const DCString& typehint=NULL) {
    IRefObject::P ro;
    if(!obj_get(media_name, ro, SObjOptions(T::_root_get_interface_type(), f, this, par, typehint)))
      return false;
    if(!ro->t_ref_get_other_interface(obj))
      return false;
    return true;
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibrary, 2, IRefObject);
};

//***************************************
// core media utilities
//***************************************

inline bool is_extension_case(const DCString& name, const DCString& ext) 
{
  if(~name<~ext)
    return false;
  if(tl::su::scmp(name.text+~name-~ext, ext.text, ~ext)==0)
    return true;
  return false;
}

inline bool is_extension_nocase(const DCString& name, const DCString& ext) 
{
  if(~name<~ext)
    return false;
  if(tl::su::seq_nocase(name.text+~name-~ext, ext.text, ~ext))
    return true;
  return false;
}

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
