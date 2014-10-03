#ifndef __UMODSYS_BASE_RSYSTEM_H
#define __UMODSYS_BASE_RSYSTEM_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rsystem.h
// info: implementation of base system object
/*************************************************************/

#include "umodsys.basesys.rcommon.h"
#include "umodsys.basesys.rmodule.h"
#include "umodsys.basesys.rparameters.h"
#include "umodsys.basesys.rexec.h"

namespace UModSys {
namespace base {
namespace rsystem {

//***************************************
// ISystem
//***************************************

struct RSystem : 
  public ISystem,
  public IUniquePointerResolver,
  public ISourceContextAdapter,
  public IExecutorPure
{
public:
  enum { 
    upi_quant = 0x100, 
    sc_quant  = 0x100
  };
  //
/*
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
*/
  typedef tl::TQuantArrayUnique<
    SUniquePointerInfo,
    upi_quant,
    core::SMemAlloc_Malloc
  > DUniPtrArray;
  //
  typedef tl::TQuantArrayUnique<
    SSourceContext,
    sc_quant,
    core::SMemAlloc_Malloc
  > DSrcContextArray;
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
  const core::SSourceContext* persist_ctx(const core::SSourceContext* sc);
public:
  bool get_var(const core::DCString& name, core::DCString& value) const;
  bool get_shell(const core::DCString& name, IShell::P& shell) const;
  //
  bool execute(const core::DCString& text);
  bool set_var(const core::DCString& name, const core::DCString& value, bool can_new);
  bool set_shell(const core::DCString& name, IShell* shell);
public:
  ISourceContextAdapter* get_sca(void);
  IUniquePointerResolver* get_upr(void);
  IModuleLoader* get_modloader(void);
  IModuleLibrary* get_syslib(void);
  IMemAlloc* get_sysmem(void);
  IMemAlloc* get_sharemem(void);
  IConsole* get_console(void);
  IParameters* get_params(void);
  IExecutorPure* get_executor(void);
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
  RParameters::SelfP params;
  //
  DSystemStaticPool uptr_strings;
  DUniPtrArray uptr_list;
  //
  DSystemStaticPool sc_strings;
  DSrcContextArray sc_list;
  //
  DRMemAlloc mema_shared, mema_system;
  //
  void set_console(IConsole* console);
  bool init(void);
  bool exec_args(int argc, char** argv);
  bool exec_main(void);
  bool exec_tests(void);
  bool deinit(void);
  //
  void dump_upis(void);
  //
  size_t find_shells(IRefObject::TypeId tids[], size_t ntids, BCStr mask);
  //
  const DCString& uptr_string(const DCString& v) { return *uptr_strings.append(v); }
  const DCString& sc_string(const DCString& v) { return *sc_strings.append(v); }
  const DCString& mod_string(const DCString& v) { return moddb.get_string(v); }
public:
  bool exec_test_shells(void);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(UModSys::base::rsystem::RSystem, 2, ISystem);
public:
  static RSystem s_sys;
};

template<typename Header, typename SubAllocator>
const core::SSourceContext* TRMemAlloc_CC<Header, SubAllocator>::persist_ctx(const core::SSourceContext* sc)
{
  return RSystem::s_sys.persist_ctx(sc);
}

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RSYSTEM_H
