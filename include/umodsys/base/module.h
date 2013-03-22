#ifndef __UMODSYS_BASE_MODULE_H
#define __UMODSYS_BASE_MODULE_H 1
/*************************************************************/
// file: umodsys/base/module.h
// info: base module object
/*************************************************************/

#include <umodsys/core/stdcore.h>

namespace UModSys {
namespace base {

struct ISystem;

struct IModObject;
struct IGenerator;

//***************************************
// SModuleInfo
//***************************************

struct SModuleInfo {
  core::DCString name;
  core::SVersion verno;
  core::DCString info;
  //
  ~SModuleInfo(void);
  SModuleInfo(const char* n, int vh, int vl, const char *i);
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
  virtual bool is_open(void) const =0;
  virtual bool open(void) =0;
  virtual bool close(void) =0;
  //
  virtual bool reg(IModObject* mo, bool doreg) =0;
  virtual bool reg(IGenerator* gen, bool doreg) =0;
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
  virtual ISystem* get_system(void) const =0;
  virtual size_t get_module_count(void) const =0;
  virtual IModule* get_module(size_t id) const =0;
  //
  virtual bool lib_loaded(void) const =0;
  virtual bool lib_load(void) =0;
  virtual bool lib_unload(void) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IModuleLibrary, 2, IRefObject);
};

//***************************************
// IModuleLoader
//***************************************

struct IModuleLoader
{
public:
  virtual const core::DCString& moduledb_get_string(const core::DCString &s) =0;
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
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_MODULE_H
