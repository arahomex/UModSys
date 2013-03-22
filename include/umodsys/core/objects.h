#ifndef __UMODSYS_CORE_OBJECTS_H
#define __UMODSYS_CORE_OBJECTS_H 1
/*************************************************************/
// file: umodsys/core/objects.h
// info: objects and interface bases
/*************************************************************/

#include <umodsys/core/objects_def.h>

namespace UModSys {
namespace core {

//***************************************
// IRoot - all virtual classes base
//***************************************

struct IRoot {
  typedef HUniquePointer TypeId;
  typedef SUniquePointerInfo TypeInfo;
  typedef SUniquePtrList DPtrList;
  typedef int DVersionNo;
  //
  virtual ~IRoot(void);
  virtual const TypeInfo& get_interface_info(void) const =0;
  virtual TypeId get_interface_type(void) const =0;
  virtual const IRoot* get_other_interface(TypeId type) const =0;
  virtual IRoot* get_other_interface(TypeId type) =0;
  virtual size_t get_interface_types(DPtrList& list) const =0;
  // INLINES
  template<typename RData>
  inline bool t_get_other_interface(RData* &rv, TypeId type) {
    rv = static_cast<RData*>( get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_get_other_interface(RData* &rv) {
    rv = static_cast<RData*>( get_other_interface(RData::_get_interface_type()) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_get_other_interface(const RData* &rv, TypeId type) const {
    rv = static_cast<const RData*>( get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_get_other_interface(const RData* &rv) const {
    rv = static_cast<const RData*>( get_other_interface(RData::_get_interface_type()) );
    return rv!=NULL;
  }
  // INLINES
  template<typename RData>
  inline bool t_get_other_interface_n(RData* &rv, TypeId type) {
    rv = static_cast<RData*>( get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_get_other_interface_n(RData* &rv) {
    rv = static_cast<RData*>( get_other_interface(RData::_get_interface_name()) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_get_other_interface_n(const RData* &rv, TypeId type) const {
    rv = static_cast<const RData*>( get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_get_other_interface_n(const RData* &rv) const {
    rv = static_cast<const RData*>( get_other_interface(RData::_get_interface_name()) );
    return rv!=NULL;
  }
protected:
  inline const IRoot* _get_other_interface(TypeId type) const { return type==_get_interface_type() ? this : NULL; }
  inline IRoot* _get_other_interface(TypeId type) { return type==_get_interface_type() ? this : NULL; }
public:
  inline static const char* _get_interface_infoname(void) { return "core::IRoot"; } \
  inline static DVersionNo _get_interface_infover(void) { return 2; } \
  inline static TypeId _get_interface_type(void) { return tl::TObjectUniqueID<IRoot>::get_id(); }
  inline static const TypeInfo& _get_interface_info(void) { return tl::TObjectUniqueID<IRoot>::get_info(); }
  inline static bool _is_interface_supported(TypeId type) { return type==_get_interface_type(); }
  inline static size_t _get_interface_types(DPtrList& list) { list<<_get_interface_type(); return 1; }
  inline const IRoot* _get_interface_p(void) const { return this; }
  inline IRoot* _get_interface_p(void) { return this; }
};

//***************************************
// IRefObject - all reference classes base
//***************************************

struct IRefObject : public IRoot {
  ~IRefObject(void);
  virtual void ref_add(void) const =0;
  virtual void ref_remove(void) const =0;
  virtual int  ref_links(void) const =0;
public:
  template<typename RData>
  inline bool t_get_other_interface_ref(tl::TRefObject<RData> &rv) {
    rv = static_cast<RData*>( get_other_interface(RData::_get_interface_type()) );
    return rv.valid();
  }
  template<typename RData>
  inline bool t_get_other_interface_ref(tl::TRefObject<const RData> &rv) const {
    rv = static_cast<const RData*>( get_other_interface(RData::_get_interface_type()) );
    return rv.valid();
  }
  template<typename RData>
  inline bool t_get_other_interface_ref_n(tl::TRefObject<RData> &rv) {
    rv = static_cast<RData*>( get_other_interface(RData::_get_interface_name()) );
    return rv.valid();
  }
  template<typename RData>
  inline bool t_get_other_interface_ref_n(tl::TRefObject<const RData> &rv) const {
    rv = static_cast<const RData*>( get_other_interface(RData::_get_interface_name()) );
    return rv.valid();
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(core::IRefObject, 2, IRoot);
};

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_OBJECTS_H
