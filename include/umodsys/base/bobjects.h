#ifndef __UMODSYS_BASE_OBJECTS_H
#define __UMODSYS_BASE_OBJECTS_H 1
/*************************************************************/
// file: umodsys/base/bobjects.h
// info: base objects
/*************************************************************/

#include <umodsys/core/objects.h>
#include <umodsys/base/module.h>
#include <umodsys/tl/util/type_constructor.h>

namespace UModSys {
namespace base {

using core::SParameters;
using core::DCString;
using core::BCStr;
using core::IRefObject;

//***************************************
// DEFINES
//***************************************

#define UMODSYS_REFOBJECT_REFMODULE() \
  typedef IModObject DOwner; typedef tl::TRefObject<IModObject> DOwnerP; \
  DOwner *owner; \
  inline void rc_init(DOwner *own) { owner=own; ref_count=0; owner->mod_inc(); } \
  inline virtual void suicide(void) { DOwnerP p(owner, void_null()); owner=NULL; delete this; p->mod_dec();  }

#define UMODSYS_BASE_SHELL_DEFINE(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  UMODSYS_REFOBJECT_REFMODULE()

#define UMODSYS_BASE_GENERATOR_DEF(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  inline void rc_init(void) {} \
  inline virtual void suicide(void) {} \
  IModule* get_module(void) const; \
  TypeId get_uid(void) const { return s_uid; } \
  bool mod_inc(void) const; \
  bool mod_dec(void) const; \
  typedef tl::TTypeStaticHolder<_type> Holder; \
  static core::SUniquePointer s_uid; \
  /*static Holder s_gen;*/ \
  static bool s_reg(IModuleReg* imr); \
  static bool s_unreg(IModuleReg* imr); \

#define UMODSYS_BASE_MODOBJECT_FUNC(_type, _mod) \
  IModule* _type::get_module(void) const { return _mod::s_modreg.module; } \
  bool _type::mod_inc(void) const { return _mod::s_modreg.module->open(); } \
  bool _type::mod_dec(void) const { return _mod::s_modreg.module->close(); } \
  core::SUniquePointer _type::s_uid("modobject", #_type ":unique", 0); \
  /*_type::Holder _type::s_gen;*/ \
  _type::Holder _type##__s_gen; \
  bool _type::s_reg(IModuleReg* imr) { _type##__s_gen.init(); return imr->reg(_type##__s_gen); } \
  bool _type::s_unreg(IModuleReg* imr) { bool rv = imr->unreg(_type##__s_gen); _type##__s_gen.init(); return rv; } \
  /*bool _type::s_reg(IModuleReg* imr) { s_gen.init(); return imr->reg(s_gen); }*/ \
  /*bool _type::s_unreg(IModuleReg* imr) { bool rv = imr->unreg(s_gen); s_gen.init(); return rv; }*/\


#define UMODSYS_BASE_GENERATOR_FUNC(_type, _mod) \
  UMODSYS_BASE_MODOBJECT_FUNC(_type, _mod)
  

#define UMODSYS_BASE_MODREG_DEF(_type) \
  static _type s_modreg; static SModuleRegChain s_modreg_chain;

#define UMODSYS_BASE_MODREG_BODY(_type) \
  _type _type::s_modreg; SModuleRegChain _type::s_modreg_chain(_type::s_modreg);


//***************************************
// IModObject::  - per-module class base
//***************************************

struct IModObject : public IRefObject {
  virtual IModule* get_module(void) const =0;
  virtual TypeId get_uid(void) const =0;
  virtual bool mod_inc(void) const =0;
  virtual bool mod_dec(void) const =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IModObject, 2, IRefObject);
};

//***************************************
// IGenerator::  - generator class base
//***************************************

struct IGenerator : public IModObject {
  virtual int get_generated_names(DPtrList& list) const =0; // only names
  virtual int get_generated_types(DPtrList& list, TypeId name) const =0; // may have different types per name
  virtual bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) =0;
protected:
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IGenerator, 2, IModObject);
};

//***************************************
// IShell::  - main module functionality
//***************************************

struct IShell : public IRefObject {
  virtual bool connect(void) =0;
  virtual bool disconnect(void) =0;
  virtual bool process_tick(const core::STimeMsec& delta) =0;
  virtual bool process_command(int argc, const core::DCString argv[]) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IShell, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_OBJECTS_H
