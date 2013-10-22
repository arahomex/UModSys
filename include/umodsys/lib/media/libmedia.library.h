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
  struct SMountPoint {
    int pm;
    IBinArchive::P archive;
    //
    inline SMountPoint(IBinArchive* a=NULL, int p=mp_All) : archive(a), pm(p) {}
  };
public:
  // sub-archives
  virtual SMountPoint mount_get(const DCString& mountpoint, int order=0) const =0;
  virtual bool mount_add(const SMountPoint& mp, const DCString& mountpoint, int order=0) =0;
  virtual bool mount_remove(const DCString& mountpoint, int order=0) =0;
  virtual bool mount_clear(void) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryBinTree, 2, ILibrary);
};

struct ILibraryBinCache
: public ILibrary
{
public:
  // binary cache
  virtual bool bincache_get(SCMemShared& mem, const DCString& media_name) const =0; // get cache element
  virtual bool bincache_add(const SCMemShared& mem, const DCString& media_name) =0; // put cache element
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
  virtual bool filters_add(IBinObjFilter* filter, const SParameters* filter_params=NULL, int order=0) =0;
  virtual bool filters_remove(IBinObjFilter* filter, int order=0) =0;
  virtual bool filters_clear(void)=0; // clear all cache
  virtual bool filters_load(IRefObject::P& obj, const IBinObjFilter::SInfo& info) =0;
  virtual bool filters_load(IRefObject* obj, const IBinObjFilter::SInfo& info) =0;
  virtual bool filters_save(IRefObject* obj, IBinObjFilter::SInfo& info) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibraryObjFilter, 2, ILibrary);
};

struct ILibraryObjectCache 
: public ILibrary
{
public:
  // object cache 
  virtual bool objcache_get(IRefObject::P& obj, const DCString& media_name) const =0; // get cache element
  virtual bool objcache_add(IRefObject* obj, const DCString& media_name) =0; // put cache element
  virtual bool objcache_remove(const DCString& media_name)=0; // clear cache element
  virtual bool objcache_clear(void)=0; // clear all cache
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibrary, 2, ILibrary);
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
