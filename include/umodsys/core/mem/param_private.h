#ifndef __UMODSYS_CORE_PARAM_PRIVATE_H
#define __UMODSYS_CORE_PARAM_PRIVATE_H 1
/*************************************************************/
// file: umodsys/core/param/param_private.h
// info: universal memory parameters, private
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/core/stdstrings.h>
#include <umodsys/core/objects.h>
#include <umodsys/core/mem/mem_fixed.h>

namespace UModSys {
namespace core {

struct IParameters;                      // any parameters worker

struct SParametersData;                  // any parameters base
struct SParameters;                      // any parameters

template<int nsize> struct TParametersA; // fixed parameters in stack

//***************************************
// IParameters::
//***************************************

struct IParameters : public core::IRefObject {
  // SParametersData
  virtual void p_deinit(SParametersData *p) =0;
  virtual void p_init(SParametersData *p, int defalloc) =0;
  virtual void p_init(SParametersData *p, const SParametersData& r) =0;
  virtual void p_init_s(SParametersData *p, void *buffer, int nalloc) =0; // static
  virtual void p_init_s(SParametersData *p, const SParametersData& r, void *buffer, int nalloc) =0; // static
  virtual void p_copy(SParametersData *p, const SParametersData& r) =0;
  //
  virtual bool add(SParametersData *p, BCStr name, BCStr value) =0;
  virtual bool add(SParametersData *p, BCStr name, const DCString& value) =0;
  virtual bool add(SParametersData *p, BCStr name, int value) =0;
  virtual bool add(SParametersData *p, BCStr name, const double &value) =0;
  virtual bool add(SParametersData *p, BCStr name, IRefObject *value) =0;
  //
  virtual bool get(const SParametersData *p, BCStr name, BCStr &value) =0;
  virtual bool get(const SParametersData *p, BCStr name, DCString& value) =0;
  virtual bool get(const SParametersData *p, BCStr name, int &value) =0;
  virtual bool get(const SParametersData *p, BCStr name, double &value) =0;
  virtual bool get(const SParametersData *p, BCStr name, IRefObject* &value) =0;
  //
  virtual bool next(const SParametersData *p, BCStr& name) =0;
  virtual bool next(const SParametersData *p, DCString& name) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::core::IParameters, 2, IRefObject);
};

//***************************************
// SParametersData::
//***************************************

struct SParametersData {
  IParameters* worker;
  SMemChunk amem; // if allocated
  SMem mem;
  int mem_used;
  //
  inline SParametersData(void) : worker(NULL) {}
};

//***************************************
// SParameters::
//***************************************

struct SParameters {
public:
protected:
  SParametersData data;
  //
  static IParameters* get_paramworker(void);

public:
  inline ~SParameters(void) 
    { if(data.worker) data.worker->p_deinit(&data); }
  inline SParameters(int defalloc=1024) 
    { get_paramworker()->p_init(&data, defalloc); }
  inline SParameters(const SParameters& r) 
    { if(r.data.worker) r.data.worker->p_init(&data, r.data); }
  inline SParameters(IParameters* w, void *buffer, int nalloc) 
    { w->p_init_s(&data, buffer, nalloc); }
  inline SParameters(const SParameters& r, void* buffer, int nalloc) 
    { if(r.data.worker) r.data.worker->p_init_s(&data, r.data, buffer, nalloc); }
  inline const SParameters& operator=(const SParameters& r) 
    { data.worker->p_copy(&data, r.data); return *this; }
  inline const SParameters& operator=(const SParameters* r) 
    { if(r) data.worker->p_copy(&data, r->data); return *this; }
  //
  inline bool add(BCStr name, BCStr value) 
    { return data.worker ? data.worker->add(&data, name, value) : false; }
  template<typename Core, typename Cmp> inline bool add(BCStr name, const tl::TString<Core, Cmp>& value) 
    { return data.worker ? data.worker->add(&data, name, value.str()) : false; }
  inline bool add(BCStr name, int value) 
    { return data.worker ? data.worker->add(&data, name, value) : false; }
  inline bool add(BCStr name, bool value) 
    { return data.worker ? data.worker->add(&data, name, int(value)) : false; }
  inline bool add(BCStr name, const double &value) 
    { return data.worker ? data.worker->add(&data, name, value) : false; }
  inline bool add(BCStr name, const float& value) 
    { return data.worker ? data.worker->add(&data, name, double(value)) : false; }
  inline bool add(BCStr name, IRefObject *value) 
    { return data.worker ? data.worker->add(&data, name, value) : false; }
  template<typename IRef, typename Func> inline bool add(BCStr name, const tl::TRefObject<IRef, Func>& value) 
    { return data.worker ? data.worker->add(&data, name, value()) : false; }
  //
  inline bool get(BCStr name, BCStr &value) const
    { return data.worker ? data.worker->get(&data, name, value) : false; }
  inline bool get(BCStr name, DCString &value) const
    { return data.worker ? data.worker->get(&data, name, value) : false; }
  inline bool get(BCStr name, int &value) const
    { return data.worker ? data.worker->get(&data, name, value) : false; }
  inline bool get(BCStr name, bool &value) const { 
    int v2;
    if(!data.worker || !data.worker->get(&data, name, v2))
      return false;
    value = v2!=0; return true; 
  }
  inline bool get(BCStr name, double &value) const
    { return data.worker ? data.worker->get(&data, name, value) : false; }
  inline bool get(BCStr name, float &value) const { 
    double v2;
    if(!data.worker || !data.worker->get(&data, name, v2))
      return false;
    value = v2; return true; 
  }
  inline bool get(BCStr name, IRefObject* &value) const
    { return data.worker ? data.worker->get(&data, name, value) : false; }
  template<typename IRef, typename RefFunc> 
  inline bool ref_get(BCStr name, tl::TRefObject<IRef, RefFunc>& value) const { 
    IRefObject* v;
    return data.worker && data.worker->get(&data, name, v) && v && v->t_ref_get_other_interface(value);
  }
  template<typename IRef> 
  inline bool ref_get(BCStr name, IRef*& value) const { 
    IRefObject* v;
    return data.worker && data.worker->get(&data, name, v) && v && v->t_root_get_other_interface(value);
  }
  //
  inline bool enum_names(BCStr &name) const
    { return data.worker ? data.worker->next(&data, name) : false; }
  inline bool enum_names(DCString &name) const
    { return data.worker ? data.worker->next(&data, name) : false; }
};

//***************************************
// TParametersA::
//***************************************

template<int nsize>
struct TParametersA : public SParameters {
  byte buffer[nsize];
public:
  inline TParametersA(IParameters* w) 
    : SParameters(w, buffer, nsize) {}
  inline TParametersA(void) 
    : SParameters(get_paramworker(), buffer, nsize) {}
  inline TParametersA(const SParameters& r) 
    : SParameters(r, buffer, nsize) {}
  inline TParametersA(const TParametersA& r) 
    : SParameters(r, buffer, nsize) {}
  inline const TParametersA& operator=(const SParameters& r) 
    { SParameters::operator=(r); return *this; }
  inline const TParametersA& operator=(const TParametersA& r) 
    { SParameters::operator=(r); return *this; }
  inline const TParametersA& operator=(const SParameters* r) 
    { if(r) SParameters::operator=(&r); return *this; }
};

//***************************************
// OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_PARAM_PRIVATE_H
