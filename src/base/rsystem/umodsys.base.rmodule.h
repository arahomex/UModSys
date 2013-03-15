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
  UMODSYS_REFOBJECT_IMPLEMENT1(base::RModule, 2, IModule);
  UMODSYS_REFOBJECT_UNIIMPLEMENT_DEF()
  UMODSYS_REFOBJECT_REFOTHER(RModuleLibrary)
};

//***************************************
// RModuleLibrary
//***************************************

struct RModuleLibrary : public IModuleLibrary
{
public:
  struct PFD_Data;
  typedef tl::TArray<RModule::SelfP> Modules;
  //
public:
  RModuleLibrary(PFD_Data* pfd);
  RModuleLibrary(void);
  ~RModuleLibrary(void);
public:
  DCString get_sys_libname(void) const;
  IMemAlloc* get_privmem(void) const;
  size_t get_module_count(void) const;
  IModule* get_module(size_t id) const;
  //
  bool lib_loaded(void) const;
  bool lib_load(void);   
  bool lib_unload(void);
public:
  Modules modules;
  DStringSharedMalloc sys_libname;
  //
  /* platform-dependent data */
  int pfd_data[16]; 
  /* platform-dependent functions */
  static bool pfd_init(PFD_Data* pfd);
  static bool pfd_init(PFD_Data* pfd, PFD_Data* pfdR);
  static bool pfd_deinit(PFD_Data* pfd);
  static bool pfd_load(PFD_Data* pfd, const core::DCString& filename);
  static bool pfd_unload(PFD_Data* pfd);
  static bool pfd_is_loaded(const PFD_Data* pfd);
  static size_t pfd_scan(RModuleLibraryArray& la, const core::DCString& mask);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(base::RModuleLibrary, 2, IModuleLibrary);
  UMODSYS_REFOBJECT_UNIIMPLEMENT_DEF()
  UMODSYS_REFOBJECT_SINGLE()
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
