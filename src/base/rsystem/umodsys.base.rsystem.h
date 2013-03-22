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
  public IUniquePointerResolver,
  public IModuleLoader
{
public:
  enum { 
    upi_quant = 0x100 
  };
  typedef tl::TStaticStringPool<
    core::BChar,
    tl::su::TComparerBinaryHash<core::BChar>,
    core::SMemAlloc_Malloc
  > DSystemStaticPool;
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
  const DCString& moduledb_get_string(const DCString &s);
  size_t moduledb_lib_count(void);
  IModuleLibrary* moduledb_lib_get(size_t id) const;
  bool moduledb_lib_drop(IModuleLibrary* lib);
  size_t moduledb_module_count(void);
  IModule* moduledb_module_get(size_t id) const;
  IModule* moduledb_find(const core::DCString& name, const core::SVersion& verno) const;
  size_t moduledb_cleanup(void);
  void moduledb_clear(void);
  bool moduledb_load(const core::DCString& cachepath);
  bool moduledb_save(const core::DCString& cachepath);
  size_t moduledb_scan(const core::DCString& mask, bool docleanup);
public:
  IConsole* console;
  //
  DSystemStaticPool uptr_pool;
  DUniPtrArray uptr_list;
  //
  DSystemStaticPool mod_pool;
  RModuleLibraryArray mod_list;
  //
  void set_console(IConsole* console);
  bool init(void);
  bool exec_args(int argc, char** argv);
  bool exec_main(void);
  bool deinit(void);
  //
  const DCString& uptr_string(const DCString& v);
  const DCString& mod_string(const DCString& v);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(base::rsystem::RSystem, 2, ISystem);
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
