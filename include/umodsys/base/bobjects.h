#ifndef __UMODSYS_BASE_OBJECTS_H
#define __UMODSYS_BASE_OBJECTS_H 1
/*************************************************************/
// file: umodsys/base/bobjects.h
// info: base objects
/*************************************************************/

#include <umodsys/base/basetypes.h>
#include <umodsys/base/module.h>

namespace UModSys {
namespace base {

//***************************************
// DEFINES
//***************************************

#define UMODSYS_REFOBJECT_REFMODULE() \
  typedef IModObject DOwner; typedef tl::TRefObject<IModObject> DOwnerP; \
  tl::TRefObjectLinksModObject<Self, IModObject> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \

#define UMODSYS_BASE_SHELL_IMPLEMENT(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_REFMODULE()

#define UMODSYS_BASE_GENERATOR_IMPLEMENT(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  tl::TRefObjectLinks<Self> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT0() \
  inline virtual void suicide(void) {} \
  IModule* get_module(void) const; \
  TypeId get_uid(void) const { return s_uid; } \
  bool mod_inc(void) const; \
  bool mod_dec(void) const; \
  typedef tl::TTypeStaticHolder<_type> Holder; \
  static core::SUniquePointer s_uid; \
  static Holder& s_holder(void); \
  static bool s_reg(IModuleReg* imr); \
  static bool s_unreg(IModuleReg* imr); \
  static const char* s_name_uid(void) { return #_type ":unique"; }

#define UMODSYS_BASE_MODOBJECT_BODY(_type, _mod) \
  IModule* _type::get_module(void) const { return _mod::s_modreg.module; } \
  bool _type::mod_inc(void) const { return _mod::s_modreg.module->open(); } \
  bool _type::mod_dec(void) const { return _mod::s_modreg.module->close(); } \
  core::SUniquePointer _type::s_uid("modobject", _type::s_name_uid(), 0); \
  _type::Holder& _type::s_holder(void) { static _type::Holder h; return h; } \
  bool _type::s_reg(IModuleReg* imr) { s_holder().init(); return imr->reg(s_holder()); } \
  bool _type::s_unreg(IModuleReg* imr) { bool rv = imr->unreg(s_holder()); s_holder().deinit(); return rv; }


#define UMODSYS_BASE_GENERATOR_BODY(_type, _mod) \
  UMODSYS_BASE_MODOBJECT_BODY(_type, _mod)
  

#define UMODSYS_BASE_MODREG_DEF(_type) \
  static _type s_modreg; static SModuleRegChain s_modreg_chain; \
  void so_reg(void);

#define UMODSYS_BASE_MODREG_BODY(_type) \
  _type _type::s_modreg; SModuleRegChain _type::s_modreg_chain(_type::s_modreg); \
  void _type::so_reg(void) { core::lost(&so_entry_enabled_flag); }


//***************************************
// IModObject::  - per-module class base
//***************************************

struct IModObject : public IRefObject {
  virtual IModule* get_module(void) const =0;
  virtual TypeId get_uid(void) const =0;
  virtual bool mod_inc(void) const =0;
  virtual bool mod_dec(void) const =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IModObject, 2, IRefObject);
};

//***************************************
// IGenerator::  - generator class base
//***************************************

struct IGenerator : public IModObject {
  virtual int get_generated_names(DPtrList& list) const =0; // only names
  virtual int get_generated_types(DPtrList& list, TypeId name) const =0; // may have different types per name
  virtual bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) =0;
public:
  template<typename RData>
  inline bool t_generate(tl::TRefObject<RData> &rv, TypeId name, const SParameters& args) {
    IRefObject::P& obj;
    if(!generate(obj, name, args) || !obj.valid())
      return false;
    if(!obj->t_ref_get_other_interface(rv))
      return false;
    return true;
  }
protected:
  template<typename Type> 
  static int t_names(DPtrList& list) { 
    list<<Type::_root_get_interface_type(); return 1; 
  }
  //
  template<typename Type> 
  static bool t_types(int &rv, DPtrList& list, TypeId name) {
    if(name==Type::_root_get_interface_type()) {
      rv = Type::_root_get_interface_types(list);
      return true;
    }
    return false;
  }
  //
  template<typename Type, typename TOwn> 
  bool t_gen_param(TOwn* own, IRefObject::P& obj, TypeId name, const SParameters& args) {
    if(name==Type::_root_get_interface_type()) {
      return core::ValidateConstruction(obj, new(local_memory()) Type(own, args));
    }
    return false;
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IGenerator, 2, IModObject);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_OBJECTS_H
