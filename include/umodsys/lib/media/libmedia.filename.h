#ifndef __UMODSYS_LIBMEDIA_FILENAME_H
#define __UMODSYS_LIBMEDIA_FILENAME_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/filename.h
// info: media filenames
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
  inline operator const BCStr(void) const { return *name; }
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
  operator DCString(void) const { return filename.get_s(); }
  BCStr get_bs(void) const { return filename.get_text(); }
  DCString operator*(void) const { return filename.get_s(); }
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
  valid = path_replacename(filename, *upath, *uname, false); 
  return valid;
}

inline bool SVComplexFileName::next(void) 
{
  valid = path_replacenamepath(filename, *upath, *uname, ++iterid, false);
  return valid;
}

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_FILENAME_H
