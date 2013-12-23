#ifndef __UMODSYS_LIBMEDIA_COMMON_H
#define __UMODSYS_LIBMEDIA_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media common structures
/*************************************************************/

#include <umodsys/lib/media/libmedia.typedefs.h>
#include <umodsys/lib/media/libmedia.streams.h>
#include <umodsys/lib/media/libmedia.filename.h>

namespace UModSys {
namespace libmedia {

//***************************************
// INTERFACES
//***************************************

//***************************************
// IUtilities::

struct IUtilities
: public IRefObject 
{
public:
  struct IMemWriterCompleted {
    virtual bool completed(const SCMem &data) =0;
  };
public:
  // -- buffers
  virtual bool buffer_free(SBuffer& B, const SSourceContext* sctx) =0;
  virtual bool buffer_alloc(SBuffer& B, size_t size, const SSourceContext* sctx) =0;
  // -- sub streams
  virtual IStreamReader::P sub_reader(IStreamReader* master, DFilePosition pos, DFilePosition size) =0; 
  virtual IStreamWriter::P sub_writer(IStreamWriter* master, DFilePosition pos, DFilePosition size) =0; 
  // -- memory streams
  virtual IStreamReader::P mem_reader(const SCMem& mem) =0; // fixed place reader
  virtual IStreamReader::P mem_reader(const SCMemShared& mem) =0; // fixed place reader
         // dynamic or static writer completed or fixed writer
  virtual IStreamWriter::P mem_writer(const SMem* mem, IMemWriterCompleted* mw, IRefObject* mwh) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IUtilities, 1, IRefObject);
};

//***************************************
// ILibObject::

struct ILibObject
: public IRefObject, 
  public SFlags::ISetter
{
public:
public:
  IUtilities::P utils;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibObject, 1, IRefObject);
};

//***************************************
// IDataArchive::

struct IBinArchive
: public ILibObject
{
public:
  virtual IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags=SFlags()) =0;
  virtual bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags=SFlags()) =0;
  virtual bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags=SFlags()) =0;
  virtual bool data_list(const DCString& mask, size_t namestart, DIFileInfoArray& list, const SFlags& flags=SFlags()) = 0;
  virtual bool data_info(const DCString& mask, size_t namestart, SFileInfo& list, const SFlags& flags=SFlags()) = 0;
  virtual int get_permissions(void) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinArchive, 2, ILibObject);
};

//***************************************
// IObjectFilter::

struct IBinObjFilter 
: public ILibObject
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
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinObjFilter, 2, ILibObject);
};


//***************************************
// ILibrary:

struct ILibrary
: public ILibObject
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
  virtual IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags);
  virtual IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags);
  virtual bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags); // load binary data
  virtual bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags); // save binary data
  virtual bool bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags); // determine attributes
  virtual bool bin_list(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags); // get full list
  virtual bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv=false); // get cache element
  virtual bool bin_put(const DCString& media_name, const SCMemShared* mem); // put cache element
  // general object functions
  virtual bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj);
  virtual bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj);
  virtual bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj);
  virtual bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv=false); // get cache element
  virtual bool obj_cput(const DCString& media_name, IRefObject* obj); // put cache element
  // universal object functions
  virtual bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts);
  virtual bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts);
  virtual bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts);
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
  inline bool bin_list(const DCString& media_name, DIFileInfoArray& info, const DMediaFlags& flags = DMediaFlags()) {
    return bin_list(media_name, info, SFlags(flags, this));
  }
  inline bool bin_info(const DCString& media_name, SFileInfo& info, const DMediaFlags& flags = DMediaFlags()) {
    return bin_info(media_name, info, SFlags(flags, this));
  }
  //
  inline bool obj_load(const DCString& media_name, IRefObject* obj,
                       const DCString& typehint = NULL,
                       const SParameters* params = NULL, 
                       const DMediaFlags& flags = DMediaFlags()) {
    return obj_load(media_name, obj, SObjOptions(NULL, flags, this, params, typehint));
  }                      
  inline bool obj_save(const DCString& media_name, const IRefObject* obj, 
                       const DCString& typehint = NULL,
                       const SParameters* params = NULL, 
                       const DMediaFlags& flags = DMediaFlags()) {
    return obj_save(media_name, obj, SObjOptions(NULL, flags, this, params, typehint));
  }
  inline bool obj_get(const DCString& media_name, IRefObject::P& obj, 
                      const DCString& typehint = NULL,
                      const SParameters* params = NULL, 
                      const DMediaFlags& flags = DMediaFlags()) {
    return obj_get(media_name, obj, SObjOptions(NULL, flags, this, params, typehint));
  }
  template<typename T>
  inline bool t_obj_get(const DCString& media_name, tl::TRefObject<T>& obj, 
                        const DCString& typehint=NULL,
                        const SParameters* par=NULL,
                        const DMediaFlags f=DMediaFlags()) {
    IRefObject::P ro;
    if(!obj_get(media_name, ro, SObjOptions(T::_root_get_interface_type(), f, this, par, typehint)))
      return false;
    if(!ro->t_ref_get_other_interface(obj))
      return false;
    return true;
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ILibrary, 2, ILibObject);
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
  if(tl::su::utf8_cmp_nocase(name.text+~name-~ext, ext.text, ~ext)==0)
    return true;
  return false;
}

//***************************************
// INLINES/OUTLINES
//***************************************


//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_COMMON_H
