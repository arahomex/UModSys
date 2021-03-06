#ifndef __UMODSYS_CORE_MODULE_H
#define __UMODSYS_CORE_MODULE_H 1
/*************************************************************/
// file: umodsys/core/module.h
// info: base module object
/*************************************************************/

#include <umodsys/core/basetypes.h>
//#include <umodsys/core/console.h>

namespace UModSys {
namespace base {

//***************************************
// SModuleInfo
//***************************************

struct SModuleInfo {
  core::DCString name;
  core::SVersion verno;
  core::DCString info;
  core::Buint32 buildno;
  core::BTime updated;
  //
  ~SModuleInfo(void);
  SModuleInfo(const char* n, core::Buint16 vh, core::Buint16 vl, const char *i, core::Buint32 bno, core::BTime u);
  SModuleInfo(void);
  void set(const char* n, int vh, int vl, const char *i);
  //
  inline bool operator==(const SModuleInfo& R) const 
    { return name==R.name && verno==R.verno; }
  inline bool eq(const core::DCString &Rname, const core::SVersion &Rverno) const 
    { return name==Rname && verno==Rverno; }
};

//***************************************
// IModule
//***************************************

struct IModule
: public core::IRefObject
{
public:
  ~IModule(void);
public:
  virtual IModuleLibrary* get_library(void) const =0;
  virtual const SModuleInfo& get_info(void) const =0;
  //
  virtual SDebug* getdebug(void) const =0;
  virtual bool is_open(void) const =0;
  virtual bool open(void) =0;
  virtual bool close(void) =0;
  //
  virtual bool reg(IModObject* mo, bool doreg) =0;
  virtual bool reg(IGenerator* gen, bool doreg) =0;
  //
  virtual size_t mod_findobjname(core::IRefObject::TypeId intr, core::IRefObject::TypeId found[], size_t nfound, core::BCStr mask) =0;
  virtual bool mod_generate(core::IRefObject::P& obj, core::IRefObject::TypeId name, const core::SParameters& args) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IModule, 2, IRefObject);
};

//***************************************
// IModuleLibrary
//***************************************

struct IModuleLibrary
: public core::IRefObject
{
public:
  ~IModuleLibrary(void);
public:
  virtual core::DCString get_sys_libname(void) const =0;
  virtual core::IMemAlloc* get_privmem(void) const =0;
  virtual IKernel* get_system(void) const =0;
  virtual size_t get_module_count(void) const =0;
  virtual IModule* get_module(size_t id) const =0;
  //
  virtual bool lib_loaded(void) const =0;
  virtual bool lib_load(void) =0;
  virtual bool lib_unload(void) =0;
  virtual bool lib_free(void) =0;
  //
  virtual size_t lib_findobjname(core::IRefObject::TypeId intr, core::IRefObject::TypeId found[], size_t nfound, core::BCStr mask) =0;
  virtual bool lib_generate(core::IRefObject::P& obj, core::IRefObject::TypeId name, const core::SParameters& args) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IModuleLibrary, 2, IRefObject);
};

//***************************************
// IModuleLoader
//***************************************

struct IModuleLoader
{
public:
  virtual ~IModuleLoader(void);
public:
  virtual size_t moduledb_findobjname(core::IRefObject::TypeId intr, core::IRefObject::TypeId found[], size_t nfound, core::BCStr mask) =0;
  virtual bool moduledb_generate(core::IRefObject::P& obj, core::IRefObject::TypeId name, const core::SParameters& args) =0;
  //
  virtual size_t moduledb_lib_count(void) =0;
  virtual IModuleLibrary* moduledb_lib_get(size_t id) const =0;
  virtual bool moduledb_lib_drop(IModuleLibrary* lib) =0;
  //
  virtual size_t moduledb_module_count(void) =0;
  virtual IModule* moduledb_module_get(size_t id) const =0;
  virtual IModule* moduledb_find(const core::DCString& name, const core::SVersion& verno) const =0;
  //
  virtual size_t moduledb_cleanup(void) =0;
  virtual void moduledb_clear(void) =0;
  virtual bool moduledb_load(const core::DCString& cachepath) =0;
  virtual bool moduledb_save(const core::DCString& cachepath) =0;
  virtual size_t moduledb_scan(const core::DCString& mask, bool docleanup=true) =0;
  virtual const core::DCString& moduledb_get_string(const core::DCString &s) =0;
public:
  template<typename RData>
  inline bool t_moduledb_generate(tl::TRefObject<RData> &rv, core::IRefObject::TypeId name, const core::SParameters& args) {
    core::IRefObject::P obj;
    if(!moduledb_generate(obj, name, args) || !obj.valid())
      return false;
    if(!obj->t_ref_get_other_interface(rv))
      return false;
    return true;
  }
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_CORE_MODULE_H
