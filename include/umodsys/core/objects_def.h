#ifndef __UMODSYS_CORE_OBJECTS_DEF_H
#define __UMODSYS_CORE_OBJECTS_DEF_H 1
/*************************************************************/
// file: umodsys/core/objects_def.h
// info: objects and interface bases defines
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/core/source.h>

#include <umodsys/tl/util/ref_pointer.h>
#include <umodsys/tl/util/object_id.h>

namespace UModSys {
namespace core {

//***************************************
// DEFINES
//***************************************

//*************************************** ANY ROOT
//***************************************

//#define UMODSYS_ROOT_LUP(_type) \
//  SUniquePointer _type::s_interface_type("typeinfo", #_type)

#define UMODSYS_ROOT_INTIMPLEMENT(_type, _verno, _interface) \
  protected: \
    const IRoot* _get_other_interface(TypeId type) const { return type==_get_interface_type() ? this : _interface::_get_other_interface(type); } \
    IRoot* _get_other_interface(TypeId type) { return type==_get_interface_type() ? this : _interface::_get_other_interface(type); } \
  public: \
    inline static const char* _get_interface_infoname(void) { return #_type; } \
    inline static DVersionNo _get_interface_infover(void) { return _verno; } \
    inline static const TypeInfo& _get_interface_info(void) { return tl::TObjectUniqueID<_type>::get_info(); } \
    inline static TypeId _get_interface_type(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
    inline static size_t _get_interface_types(DPtrList& list) { list<<_get_interface_type(); return _interface::_get_interface_types(list)+1; } \
    inline static bool _is_interface_supported(TypeId type) { return type==_get_interface_type() || _interface::_is_interface_supported(type); } \
    inline const IRoot* _get_interface_p(void) const { return this; } \
    inline IRoot* _get_interface_p(void) { return this; } \
    inline static TypeId _get_interface_basetype(void) { return _get_interface_type(); } \
    typedef _type DInterface; \
  protected: \

#define UMODSYS_ROOT_IMPLEMENT1(_type, _verno, _interface) \
  typedef _interface DParent; \
  typedef _type Self; \
  inline static const char* _get_interface_infoname(void) { return #_type; } \
  inline static DVersionNo _get_interface_infover(void) { return _verno; } \
  inline static const TypeInfo& _get_interface_info(void) { return tl::TObjectUniqueID<_type>::get_info(); } \
  inline static TypeId _get_interface_type(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
  inline static size_t _get_interface_types(DPtrList& list) { list<<_get_interface_type(); return _interface::_get_interface_types(list)+1; } \
  inline static bool _is_interface_supported(TypeId type) { return type==_get_interface_type() || _interface::_is_interface_supported(type); } \
  \
  inline const TypeInfo& get_interface_info(void) const { return _get_interface_info(); } \
  inline TypeId get_interface_type(void) const { return _get_interface_type(); } \
  inline size_t get_interface_types(DPtrList& list) const { return get_interface_types(list); } \
  inline const IRoot* get_other_interface(TypeId type) const { return type==_get_interface_type() ? _interface::_get_interface_p() : _interface::_get_other_interface(type); } \
  inline IRoot* get_other_interface(TypeId type) { return type==_get_interface_type() ? _interface::_get_interface_p() : _interface::_get_other_interface(type); } \

#define UMODSYS_ROOT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  typedef _interface1 DParent1; \
  typedef _interface2 DParent2; \
  typedef _type Self; \
  static SUniquePointer s_interface_type; \
  inline static const char* _get_interface_infoname(void) { return #_type; } \
  inline static DVersionNo _get_interface_infover(void) { return _verno; } \
  inline static const TypeInfo& _get_interface_info(void) { return tl::TObjectUniqueID<_type>::get_info(); } \
  inline static TypeId _get_interface_type(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
  inline static size_t _get_interface_types(DPtrList& list) { \
    list<<_get_interface_type(); \
    return _interface1::_get_interface_types(list)+_interface2::_get_interface_types(list)+1; \
  } \
  inline static bool _is_interface_supported(TypeId type) { \
    return type==_get_interface_type() || _interface1::_is_interface_supported(type) || _interface2::_is_interface_supported(type); \
  } \
  \
  inline const TypeInfo& get_interface_info(void) const { return _get_interface_info(); } \
  inline TypeId get_interface_type(void) const { return _get_interface_type(); } \
  inline size_t get_interface_types(DPtrList& list) const { return _get_interface_types(list); } \
  inline const IRoot* get_other_interface(TypeId type) const { \
    if(type==_get_interface_type()) return _interface1::_get_interface_p(); \
    const IRoot* rv = _interface1::_get_other_interface(type); if(rv) return rv; \
    return _interface2::_get_other_interface(type); \
  } \
  inline IRoot* get_other_interface(TypeId type) { \
    if(type==_get_interface_type()) return _interface1::_get_interface_p(); \
    IRoot* rv = _interface1::_get_other_interface(type); if(rv) return rv; \
    return _interface2::_get_other_interface(type); \
  } \

//*************************************** ANY REFOBJECT
//***************************************

#define UMODSYS_REFOBJECT_INTIMPLEMENT(_type, _verno, _interface) \
  UMODSYS_ROOT_INTIMPLEMENT(_type, _verno, _interface) \
  public: \
    typedef tl::TRefObject<_type> P, DInterfaceP; \
  protected: \

#define UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_ROOT_IMPLEMENT1(_type, _verno, _interface) \
  typedef tl::TRefObject<_type> DSelfP, SelfP; \

#define UMODSYS_REFOBJECT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_ROOT_IMPLEMENT2(_type, _interface1, _interface2) \
  typedef tl::TRefObject<_type> DSelfP, SelfP; \

#define UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  inline void ref_add(void) const { refs.ref_add(); } \
  inline void ref_remove(void) const { refs.ref_remove( const_cast<Self*>(this) ); } \
  inline int  ref_links(void) const { return refs.ref_links(); } \
  inline IMemAlloc* get_heap_allocator(void) const { return refs.heap; } \

#define UMODSYS_REFOBJECT_UNIIMPLEMENT0() \
  inline void ref_add(void) const { refs.ref_add(); } \
  inline void ref_remove(void) const { refs.ref_remove( const_cast<Self*>(this) ); } \
  inline int  ref_links(void) const { return refs.ref_links(); } \
  inline IMemAlloc* get_heap_allocator(void) const { return NULL; } \

#if 0

#define UMODSYS_REFOBJECT_SIMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  UMODSYS_REFOBJECT_PIMPLEMENT() \
  /*UMODSYS_NEW_DELETE()*/ \
  inline virtual void suicide(void) { delete this; } \

#define UMODSYS_REFOBJECT_SIMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_REFOBJECT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  UMODSYS_REFOBJECT_PIMPLEMENT() \
  /*UMODSYS_NEW_DELETE()*/ \
  inline virtual void suicide(void) { delete this; } \

#endif

//*************************************** REFOBJECT types
//***************************************

#define UMODSYS_REFOBJECT_SINGLE() \
  tl::TRefObjectLinks<Self> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  inline void suicide(void) { refs.obj_delete(this); }

#define UMODSYS_REFOBJECT_REFOTHER(_type_owner) \
  typedef _type_owner DOwner; typedef tl::TRefObject<DOwner> DOwnerP; \
  tl::TRefObjectLinksParent<Self, DOwner> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  inline void suicide(void) { DOwnerP p(refs.owner, void_null()); refs.owner = NULL; refs.obj_delete(this); }

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_OBJECTS_H
