#ifndef __UMODSYS_BASE_RMODULE_H
#define __UMODSYS_BASE_RMODULE_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rsystem.h
// info: implementation of base module object
/*************************************************************/

#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/tl/composite/array.h>

namespace UModSys {
namespace base {

using namespace core;

struct RModuleLibrary;
struct RModule;

//***************************************
// RModule
//***************************************

struct RModule : public IModule
{
public:
  RModule(RModuleLibrary *pv);
  ~RModule(void);
public:
  IModuleLibrary* get_library(void) const;
  const SModuleInfo& get_info(void) const;
  //
  bool is_open(void) const;
  bool open(void);   
  bool close(void);
public:
  IModuleReg* ireg;
  DStringSharedMalloc mi_name, mi_info;
  SModuleInfo minfo;
  int sys_id;
  //
  bool alloc_minfo(const SModuleInfo &mi2);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(RModule, 2, IModule);
  UMODSYS_REFOBJECT_UNIIMPLEMENT_DEF()
  UMODSYS_REFOBJECT_REFOTHER(RModuleLibrary)
};

//***************************************
// RModuleLibrary
//***************************************

struct RModuleLibrary : public IModuleLibrary
{
public:
  RModuleLibrary(void);
  ~RModuleLibrary(void);
public:
  DCString get_sys_libname(void) const;
  IMemAlloc* get_privmem(void) const;
  size_t get_module_count(void) const;
  IModule* get_module(size_t id) const;
  //
  bool is_loaded(void) const;
  bool load(void);   
  bool unload(void);
public:
  struct PFD_Data;
  typedef tl::TArray<RModule::SelfP> Modules;
  //
  Modules modules;
  DStringSharedMalloc sys_libname;
  //
  /* platform-dependent data */
  int pfd_data[16]; 
  /* platform-dependent functions */
  bool pfd_init(PFD_Data* pfd);
  bool pfd_deinit(PFD_Data* pfd);
  bool pfd_load(PFD_Data* pfd);
  bool pfd_unload(PFD_Data* pfd);
  bool pfd_is_loaded(const PFD_Data* pfd) const;
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(RModuleLibrary, 2, IModuleLibrary);
  UMODSYS_REFOBJECT_UNIIMPLEMENT_DEF()
  UMODSYS_REFOBJECT_SINGLE()
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
