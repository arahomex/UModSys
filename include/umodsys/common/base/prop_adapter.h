#ifndef __UMODSYS_COMMON_PROP_ADAPTER_H
#define __UMODSYS_COMMON_PROP_ADAPTER_H 1
/*************************************************************/
// file: umodsys/common/param/prop_adapter.h
// info: set/get data via specialized interface
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>
#include <umodsys/common/stdstrings.h>
#include <umodsys/common/base/obj_core.h>
#include <umodsys/common/base/mem_fixed.h>

namespace UModSys {
namespace core {

//***************************************

struct SPropValueR : tl::SDataUniqueID {
  SCMem value;
  //
  SPropValueR(const SDataUniqueID& tid, const SCMem& v) 
  : SDataUniqueID(tid), value(v) {
  }
  //
  template<typename T>
  SPropValueR(const T& tv) 
  : SDataUniqueID(tl::TDataUniqueID<T>::get_id()), 
    value(&tv, sizeof(tv)) 
  {
  }
  //
  template<typename T> inline bool is_type(void) const { 
    return id==tl::TDataUniqueID<T>::get_id() && sizeof(T)==value.size; 
  }
  template<typename T> inline const T* get_as(void) const {
    return is_type<T>() ? reinterpret_cast<const T*>(value.data) : NULL; 
  }
  template<typename T> inline bool get_as(const T* &rv) const {
    if(!is_type<T>() || value.data==NULL)
      return false;
    rv = reinterpret_cast<const T*>(value.data);
    return true;
  }
};

template<typename T>
struct TPropValueRV : SPropValueR {
  T raw;

  TPropValueRV(const T& v) 
  : raw(v), SPropValueR(raw) {
  }
};

//***************************************

struct SPropValueW : tl::SDataUniqueID {
  SCMem value;
  //
  SPropValueW(const SDataUniqueID& tid, const SMem& v) 
  : SDataUniqueID(tid), value(v) {
  }
  //
  template<typename T>
  SPropValueW(T& tv) 
  : SDataUniqueID(tl::TDataUniqueID<T>::get_id()), 
    value(&tv, sizeof(tv)) 
  {
  }
  //
  template<typename T> inline bool is_type(void) const { 
    return id==tl::TDataUniqueID<T>::get_id() && sizeof(T)==value.size; 
  }
  template<typename T> inline T* get_as(void) const {
    return is_type<T>() ? reinterpret_cast<T*>(value.data) : NULL; 
  }
  template<typename T> inline bool get_as(T* &rv) const {
    if(!is_type<T>() || value.data==NULL)
      return false;
    rv = reinterpret_cast<T*>(value.data);
    return true;
  }
};

template<typename T>
struct TPropValueWV : SPropValueW {
  T raw;

  TPropValueWV(const T& v) 
  : raw(v), SPropValueR(raw) {
  }
  TPropValueWV(void) 
  : SPropValueR(raw) {
  }
};

//***************************************

struct IPropAdapter {
  virtual bool set_value(const SPropValueR& value) =0;
  virtual bool get_value(const SPropValueW& value) const =0;
};

//***************************************

struct IPropIdAdapter {
  virtual bool set_value(HUniquePointer id, const SPropValueR& value) =0;
  virtual bool get_value(HUniquePointer id, const SPropValueW& value) const =0;
};

//***************************************

struct IPropNamedAdapter {
  virtual bool set_value(const DCString& name, const SPropValueR& value) =0;
  virtual bool get_value(const DCString& name, const SPropValueW& value) const =0;
};

//***************************************
// INLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace core

//***************************************
// INLINES (UModSys) 
//***************************************

UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Buint8);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Bsint8);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Buint16);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Bsint16);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Buint32);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Bsint32);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Bfloat32);
UMODSYS_TL_TDATAUNIQUEID_ATOM(core::Bfloat64);

//***************************************
// END
//***************************************

} // namespace UModSys

#endif
