#ifndef __UMODSYS_BASE_MODULE_H
#define __UMODSYS_BASE_MODULE_H 1
/*************************************************************/
// file: umodsys/base/module.h
// info: base module object
/*************************************************************/

#include <umodsys/core/stdcore.h>

namespace UModSys {
namespace base {

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
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IModule, 2, IRefObject);
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
  virtual size_t get_module_count(void) const =0;
  virtual IModule* get_module(size_t id) const =0;
  //
  virtual bool lib_loaded(void) const =0;
  virtual bool lib_load(void) =0;
  virtual bool lib_unload(void) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IModuleLibrary, 2, IRefObject);
};

//***************************************
// IModuleLoader
//***************************************

struct IModuleLoader
{
public:
  virtual size_t moduledb_lib_count(void) =0;
  virtual IModuleLibrary* moduledb_lib_get(size_t id) const =0;
  virtual bool moduledb_lib_drop(IModuleLibrary* lib) =0;
  //
  virtual size_t moduledb_module_count(void) =0;
  virtual IModule* moduledb_module_get(size_t id) const =0;
  virtual IModule* module_find(const core::DCString& name, const core::SVersion& verno) const =0;
  //
  virtual void moduledb_clear(void) =0;
  virtual bool moduledb_load(const core::DCString& cachepath) =0;
  virtual bool moduledb_save(const core::DCString& cachepath) =0;
  virtual size_t moduledb_scan(const core::DCString& mask) =0;
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_CONSOLE_H
