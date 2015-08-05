#ifndef __UMODSYS_LIBMEDIA_LIBRARY_H
#define __UMODSYS_LIBMEDIA_LIBRARY_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/library.h
// info: media libraries
/*************************************************************/

#include <umodsys/lib/media/libmedia.common.h>

namespace UModSys {
namespace libmedia {


//***************************************
// TYPES
//***************************************

//***************************************
// ILibraryBinTree::
// ILibraryBinCache::

struct ILibraryBinTree
: public ILibrary
{
  struct SPoint {
    int pm;
    IBinArchive::P archive;
    //
    inline SPoint(IBinArchive* a=NULL, int p=mp_All) : archive(a), pm(p) {}
  };
  //
  struct SId {
    int order;
    DCString mountpoint;
    //
    inline SId(const DCString& p, int o=0) : order(o), mountpoint(p) {}
    inline SId(int o, const DCString& p) : order(o), mountpoint(p) {}
    //
    static int compare(const SId& L, const SId& R) { 
      int rv = -scalar_compare(L.order, R.order); // revert order, a>b mean a before b
      if(rv) return rv;
      return L.mountpoint.compare(R.mountpoint);
    }
  };
public:
  // sub-archives
  virtual SPoint mount_get(const SId &sid) const =0;
  virtual bool mount_add(const SId &sid, const SPoint& mp) =0;
  virtual bool mount_remove(const SId &sid) =0;
  virtual bool mount_clear(void) =0;
public:
  inline bool mount_add(const DCString& n, const SPoint& mp) { return mount_add(SId(n), mp); }
  inline bool mount_add(const DCString& n, int o, const SPoint& mp) { return mount_add(SId(n, o), mp); }
  inline bool mount_add(const DCString& n, IBinArchive* a, int p=mp_All) { return mount_add(SId(n), SPoint(a, p)); }
  inline bool mount_add(const DCString& n, int o, IBinArchive* a, int p=mp_All) { return mount_add(SId(n, o), SPoint(a, p)); }
  //
  inline SPoint mount_get(const DCString& p, int o=0) { return mount_get(SId(p, o)); }
  inline bool mount_remove(const DCString& p, int o=0) { return mount_remove(SId(p, o)); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryBinTree, 2, ILibrary);
};

struct ILibraryBinCache
: public ILibrary
{
public:
  // binary cache
  virtual bool bincache_get(const DCString& media_name, SCMemShared* mem, bool *valid) const =0; // get cache element
  virtual bool bincache_add(const DCString& media_name, const SCMemShared& mem) =0; // put cache element
  virtual bool bincache_add(const DCString& media_name) =0; // put NULL cache element
  virtual bool bincache_remove(const DCString& media_name)=0; // clear cache element
  virtual bool bincache_clear(void)=0; // clear all cache
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryBinCache, 2, ILibrary);
};


//***************************************
// ILibraryObjFilter::
// ILibraryObjCache::

struct ILibraryObjFilter
: public ILibrary
{
public:
public:
  // object filtering
  virtual bool filters_add(IBinObjFilter::P filter, const SParameters* filter_params=NULL, int order=0) =0;
  virtual bool filters_remove(IBinObjFilter* filter, int order=0) =0;
  virtual bool filters_clear(void)=0; // clear all cache
  virtual bool filters_load(const IBinObjFilter::SInfo& info, IRefObject::P& obj) =0;
  virtual bool filters_load(const IBinObjFilter::SInfo& info, IRefObject* obj) =0;
  virtual bool filters_save(IBinObjFilter::SInfo& info, const IRefObject* obj) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryObjFilter, 2, ILibrary);
};

struct ILibraryObjCache 
: public ILibrary
{
public:
  // object cache 
  virtual bool objcache_get(const DCString& media_name, IRefObject::P* obj, bool *valid) const =0; // get cache element
  virtual bool objcache_add(const DCString& media_name, IRefObject* obj) =0; // put cache element / NULL element
  virtual bool objcache_remove(const DCString& media_name)=0; // clear cache element
  virtual bool objcache_clear(void)=0; // clear all cache
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryObjCache, 2, ILibrary);
};

//***************************************
// ILibraryLayered::

struct ILibraryLayered
: public ILibrary
{
  struct SLayer {
    int pm;
    ILibrary::P lib;
    DCString tag;
    //
    inline SLayer(ILibrary* l, int p=mp_All, const DCString& t=NULL)
      : tag(t), lib(l), pm(p) {}
  };
public:
  // sub-elements, use highest cast [IDataLibrary]
  virtual size_t layer_count(const SLayer& lay) const =0;
  virtual SLayer layer_get(size_t idx) const =0;
  virtual size_t layer_find(const DCString &tag, size_t start=array_index_none) const =0;
  virtual bool layer_insert(const SLayer& lay, size_t idx=array_index_none) =0;
  virtual bool layer_remove(size_t idx) =0;
  virtual bool layer_clear(void) =0;
  //
  inline bool layer_push(ILibrary* l, int p=mp_All, const DCString& t=NULL) {
    return layer_insert(SLayer(l, p, t));
  }
  inline bool layer_insert(size_t idx, ILibrary* l, int p=mp_All, const DCString& t=NULL) {
    return layer_insert(SLayer(l, p, t), idx);
  }
public:
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryLayered, 2, ILibrary);
};

//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_COMMON_H
