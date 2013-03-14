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
    /*inline static int _get_interface_types(DListStr& list) { list<<#_type; return _interface::_get_interface_types(list)+1; }*/ \
    inline static const char* _get_interface_cname(void) { return #_type; } \
    inline static int _get_interface_verno(void) { return _verno; } \
    inline static TypeId _get_interface_type(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
    inline static bool _is_interface_supported(TypeId type) { return type==_get_interface_type() || _interface::_is_interface_supported(type); } \
    inline const IRoot* _get_interface_p(void) const { return this; } \
    inline IRoot* _get_interface_p(void) { return this; } \
    typedef _type DInterface; \
  protected: \

#define UMODSYS_ROOT_IMPLEMENT1(_type, _verno, _interface) \
  typedef _interface DParent; \
  typedef _type Self; \
  inline static const char* _get_interface_cname(void) { return #_type; } \
  inline static int _get_interface_verno(void) { return _verno; } \
  inline static TypeId _get_interface_name(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
  inline static bool _is_interface_supported(TypeId type) { return type==_get_interface_name() || _interface::_is_interface_supported(type); } \
  inline TypeId get_interface_name(void) const { return _get_interface_name(); } \
  /*inline int get_interface_types(DListStr& list) const { list<<#_type; return _interface::_get_interface_types(list)+1; }*/ \
  inline const IRoot* get_other_interface(TypeId type) const { return type==_get_interface_name() ? _interface::_get_interface_p() : _interface::_get_other_interface(type); } \
  inline IRoot* get_other_interface(TypeId type) { return type==_get_interface_name() ? _interface::_get_interface_p() : _interface::_get_other_interface(type); } \

#define UMODSYS_ROOT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  typedef _interface1 DParent1; \
  typedef _interface2 DParent2; \
  typedef _type Self; \
  static SUniquePointer s_interface_type; \
  inline static const char* _get_interface_cname(void) { return #_type; } \
  inline static int _get_interface_verno(void) { return _verno; } \
  inline static TypeId _get_interface_name(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
  inline static bool _is_interface_supported(TypeId type) { \
    return type==_get_interface_name() || _interface1::_is_interface_supported(type) || _interface2::_is_interface_supported(type); \
  } \
  inline TypeId get_interface_name(void) const { return _get_interface_name(); } \
  /*inline int get_interface_types(DListStr& list) const { list<<#_type; return _interface::_get_interface_types(list)+1; }*/ \
  inline const IRoot* get_other_interface(TypeId type) const { \
    if(type==_get_interface_name()) return _interface1::_get_interface_p(); \
    const IRoot* rv = _interface1::_get_other_interface(type); if(rv) return rv; \
    return _interface2::_get_other_interface(type); \
  } \
  inline IRoot* get_other_interface(TypeId type) { \
    if(type==_get_interface_name()) return _interface1::_get_interface_p(); \
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
  mutable int ref_count; \
  inline void ref_add(void) const { ref_count++; } \
  inline void ref_remove(void) const { if(--ref_count==0) const_cast<Self*>(this)->suicide(); } \
  inline int  ref_links(void) const { return ref_count; } \

#define UMODSYS_REFOBJECT_UNIIMPLEMENT_DEF() \
  mutable int ref_count; \
  void ref_add(void) const; \
  void ref_remove(void) const; \
  int  ref_links(void) const; \

#define UMODSYS_REFOBJECT_UNIIMPLEMENT_BODY(_type) \
  void _type::ref_add(void) const { ref_count++; } \
  void _type::ref_remove(void) const { if(--ref_count==0) const_cast<Self*>(this)->suicide(); } \
  int  _type::ref_links(void) const { return ref_count; } \

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
  inline void rc_init(void) { ref_count=0; } \
  inline virtual void suicide(void) { delete this; }

#define UMODSYS_REFOBJECT_REFOTHER(_type_owner) \
  typedef _type_owner DOwner; typedef tl::TRefObject<DOwner> DOwnerP; \
  DOwner *owner; \
  inline void rc_init(DOwner *own) { owner=own; ref_count=0; } \
  inline virtual void suicide(void) { DOwnerP p(owner, void_null()); owner=NULL; delete this;  }

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_OBJECTS_H
