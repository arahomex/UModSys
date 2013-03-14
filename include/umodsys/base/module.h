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
  UMODSYS_REFOBJECT_INTIMPLEMENT(IModule, 2, IRefObject);
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
  virtual bool is_loaded(void) const =0;
  virtual bool load(void) =0;
  virtual bool unload(void) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(IModuleLibrary, 2, IRefObject);
};

//***************************************
// IModuleLoader
//***************************************

struct IModuleLoader
{
public:
  virtual IModule* module_preload(const core::DCString& sys_name) =0;
  virtual IModule* module_load(const core::DCString& name, const core::SVersion& verno, bool doload=true) =0;
  virtual bool module_unload(const core::DCString& name, const core::SVersion& verno) =0;
  virtual bool module_unload(IModule* M) =0;
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_CONSOLE_H
