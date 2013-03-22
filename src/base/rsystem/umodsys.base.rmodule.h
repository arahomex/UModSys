#ifndef __UMODSYS_BASE_RMODULE_H
#define __UMODSYS_BASE_RMODULE_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rsystem.h
// info: implementation of base module object
/*************************************************************/

#include "umodsys.base.rcommon.h"
#include <umodsys/tl/composite/array.h>

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

//***************************************
// RModule
//***************************************

const size_t max_generated_names = 1024;

struct RModule : public IModule
{
public:
  struct ModuleObjInfo {
    TypeId tid, uid;
    size_t start_gen, count_gen;
    int reg_count;
    IModObject* mo;
    //
    ModuleObjInfo(void) 
    : tid(NULL), uid(NULL), reg_count(0), mo(NULL), 
      start_gen(array_index_none), count_gen(array_index_none) {}
/*
    ModuleObjInfo(TypeId t, size_t s=array_index_none) 
    : tid(t), uid(t), start_gen(s), count_gen(0), reg_count(0), mo(NULL) {}
*/
  };
  struct GeneratedObjInfo {
    TypeId tid;
    size_t start_elem, count_elem;
    //
    GeneratedObjInfo(void) : tid(NULL), start_elem(array_index_none), count_elem(0) {}
  };
  //
  typedef tl::TArray<ModuleObjInfo> ModObjArray;
  typedef tl::TArray<GeneratedObjInfo> ModObjGenInfoArray;
  typedef tl::TArray<TypeId> ModObjGenTypeArray;
public:
  RModule(RModuleLibrary *pv, IModuleReg *imr);
  ~RModule(void);
public:
  IModuleLibrary* get_library(void) const;
  const SModuleInfo& get_info(void) const;
  //
  bool is_open(void) const;
  bool open(void);   
  bool close(void);
  //
  bool reg(IModObject* mo, bool doreg);
  bool reg(IGenerator* gen, bool doreg);
public:
  IModuleReg* ireg;
  SModuleInfo minfo;
  ModObjArray mos;
  ModObjGenInfoArray mogis;
  ModObjGenTypeArray mogts;
  //
  ModuleObjInfo* reg_obj(IModObject* mo);
  bool unreg_obj(IModObject* mo);
  bool reg_generator(ModuleObjInfo* mi, IGenerator* gen);
  //
  bool alloc_minfo(const SModuleInfo &mi2);
  bool scan(void);
  bool save_db(FILE *f);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(UModSys::base::rsystem::RModule, 2, IModule);
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
  typedef int PFD_Raw[16];
  typedef tl::TArray<RModule::SelfP> Modules;
public:
  RModuleLibrary(ISystem* sys, PFD_Data* pfd, IModuleLibraryReg* imlr);
  RModuleLibrary(void);
  ~RModuleLibrary(void);
public:
  DCString get_sys_libname(void) const;
  IMemAlloc* get_privmem(void) const;
  ISystem* get_system(void) const;
  size_t get_module_count(void) const;
  IModule* get_module(size_t id) const;
  //
  bool lib_loaded(void) const;
  bool lib_load(void);   
  bool lib_unload(void);
public:
  ISystem* sys;
  Modules modules;
  DStringSharedMalloc sys_libname;
  IModuleLibraryReg* ireg;
  int load_count;
  bool linked;
  //
  size_t cleanup(void);
  bool scan(void);
  bool link(void);
  bool unlink(void);
  bool load0(void);
  bool save_db(FILE *f);
  //
  static size_t s_find_dup(const RModuleLibraryArray& la, IModuleLibraryReg* ireg);
  static bool s_add(ISystem* sys, RModuleLibraryArray& la, const char *filename);
  //
//  inline bool eq(const PFD_Data* pfd2) { return pfd_eq(get_pfd(), pfd2); }
  //
  /* platform-dependent data */
  PFD_Raw raw_data;
  inline PFD_Data* get_pfd(void) { return reinterpret_cast<PFD_Data*>(raw_data); }
  inline const PFD_Data* get_pfd(void) const { return reinterpret_cast<const PFD_Data*>(raw_data); }
  //
  /* platform-dependent functions */
  static bool pfd_init(PFD_Data* pfd);
  static bool pfd_init(PFD_Data* pfd, PFD_Data* pfdR);
  //
  static IModuleLibraryReg* pfd_load(PFD_Data* pfd, const core::DCString& filename);
  static bool pfd_unload(PFD_Data* pfd);
  //
  static size_t pfd_scan(ISystem* sys, RModuleLibraryArray& la, const core::DCString& mask);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(UModSys::base::rsystem::RModuleLibrary, 2, IModuleLibrary);
  UMODSYS_REFOBJECT_UNIIMPLEMENT_DEF()
  UMODSYS_REFOBJECT_SINGLE()
};

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
