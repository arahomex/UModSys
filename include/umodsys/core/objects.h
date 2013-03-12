#ifndef __UMODSYS_CORE_OBJECTS_H
#define __UMODSYS_CORE_OBJECTS_H 1
/*************************************************************/
// file: umodsys/core/objects.h
// info: objects and interface bases
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

#define UMODSYS_REFOBJECT_UNIIMPLEMENT \
  mutable int ref_count; \
  inline void ref_add(void) const { ref_count++; } \
  inline void ref_remove(void) const { if(--ref_count==0) const_cast<Self*>(this)->suicide(); } \
  inline int  ref_links(void) const { return ref_count; } \

#define UMODSYS_REFOBJECT_PIMPLEMENT \
  UMODSYS_REFOBJECT_UNIIMPLEMENT \
  inline void rc_init(void) { ref_count=0; } \

#define UMODSYS_REFOBJECT_SIMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_PIMPLEMENT \
  /*UMODSYS_NEW_DELETE()*/ \
  inline virtual void suicide(void) { delete this; } \

#define UMODSYS_REFOBJECT_SIMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_REFOBJECT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_REFOBJECT_PIMPLEMENT \
  /*UMODSYS_NEW_DELETE()*/ \
  inline virtual void suicide(void) { delete this; } \

//***************************************
// IRoot - all virtual classes base
//***************************************

struct IRoot {
  typedef SUniquePointerInfo* TypeId;
  //
  virtual ~IRoot(void);
  virtual TypeId get_interface_name(void) const =0;
  virtual const IRoot* get_other_interface(TypeId type) const =0;
  virtual IRoot* get_other_interface(TypeId type) =0;
//  virtual int get_interface_types(DListStr& list) const =0;
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
  inline static const char* _get_interface_cname(void) { return "core::IRoot"; } \
  inline static int _get_interface_verno(void) { return 2; } \
  inline static TypeId _get_interface_type(void) { return tl::TObjectUniqueID<IRoot>::get_id(); } \
  inline static bool _is_interface_supported(TypeId type) { return type==_get_interface_type(); }
//  inline static int _get_interface_types(DListStr& list) { list<<"core::objects::IRoot"; return 1; }
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
  UMODSYS_REFOBJECT_INTIMPLEMENT(IRefObject, 2, IRoot);
};

//***************************************
// IModObject::  - per-module class base
//***************************************

struct IModObject : public IRefObject {
  virtual IModule* get_module(void) const =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(IModObject, 2, IRefObject);
};

//***************************************
// IModObject::  - generator class base
//***************************************

struct IGenerator : public IModObject {
//  virtual int get_generated_names(DListStr& list) const =0; // only names
//  virtual int get_generated_types(DListStr& list, const DHString& name) const =0; // may have different types per name
//  virtual bool generate(IRefObject::P& obj, const DHString& type, const SParameters& args) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(IGenerator, 2, IModObject);
};

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_OBJECTS_H
