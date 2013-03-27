#ifndef __UMODSYS_BASE_RSYSTEM_H
#define __UMODSYS_BASE_RSYSTEM_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rsystem.h
// info: implementation of base system object
/*************************************************************/

#include "umodsys.base.rcommon.h"
#include "umodsys.base.rmodule.h"

namespace UModSys {
namespace base {
namespace rsystem {

//***************************************
// ISystem
//***************************************

struct RSystem : 
  public ISystem,
  public IUniquePointerResolver
{
public:
  enum { 
    upi_quant = 0x100 
  };
  //
  struct SUniPtrHolder {
    size_t used;
    SUniquePointerInfo elems[upi_quant];
    SUniPtrHolder(void) : used(0) {}
  };
  typedef tl::TScatterArray<
    SUniPtrHolder, 
    int,
    tl::TNodeDeleter< 
      tl::TScatterArrayNode<SUniPtrHolder, int >,
      core::SMemAlloc_Malloc
    >
  > DUniPtrArray;
public:
  RSystem(void);
  ~RSystem(void);
public:
//  void ref_add(void) const;
//  void ref_remove(void) const;
//  int  ref_links(void) const;
public:
  HUniquePointer upi_add(const SUniquePointerInfo* lupi);
  int upi_remove(HUniquePointer upi);
public:
  IModuleLoader* get_modloader(void);
  IUniquePointerResolver* get_upr(void);
  IModuleLibrary* get_syslib(void);
  IMemAlloc* get_sysmem(void);
  IMemAlloc* get_sharemem(void);
  IConsole* get_console(void);
public:
//  core::DCString get_sys_libname(void) const;
//  core::IMemAlloc* get_privmem(void) const;
//  size_t get_module_count(void) const;
//  IModule* get_module(size_t id) const;
//  bool lib_loaded(void) const;
//  bool lib_load(void);
//  bool lib_unload(void);
public:
  IConsole* console;
  //
  RModuleLoader moddb;
  //
  DSystemStaticPool uptr_pool;
  DUniPtrArray uptr_list;
  //
  void set_console(IConsole* console);
  bool init(void);
  bool exec_args(int argc, char** argv);
  bool exec_main(void);
  bool deinit(void);
  //
  const DCString& uptr_string(const DCString& v);
  const DCString& mod_string(const DCString& v) { return moddb.get_string(v); }
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(UModSys::base::rsystem::RSystem, 2, ISystem);
public:
  static RSystem s_sys;
};

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RSYSTEM_H
