#ifndef __UMODSYS_BASE_RMODULE_H
#define __UMODSYS_BASE_RMODULE_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rsystem.h
// info: implementation of base module object
/*************************************************************/

#include <umodsys/base/system.h>
#include <umodsys/base/console.h>

namespace UModSys {
namespace base {

using namespace core;

//***************************************
// ISystem
//***************************************

struct RModule : public IModule
{
public:
  RModule(void);
  ~RModule(void);
public:
  void ref_add(void) const;
  void ref_remove(void) const;
  int  ref_links(void) const;
public:
  const DCString& get_sys_libname(void) const;
  const SModuleInfo& get_info(void) const;
  bool is_loaded(void) const;
  IMemAlloc* get_privmem(void) const;
  //
  bool load(void);   
  bool unload(void);
public:
  DStringSharedMalloc sys_libname, mi_name, mi_info;
  SModuleInfo minfo;
  int sys_id;
  //
  bool alloc_minfo(const SModuleInfo &mi2);
  //
  /* platform-dependent data */
  struct PFD_Data_t;
  int pfd_data[16]; 
  /* platform-dependent functions */
  bool pfd_init(PFD_Data_t* pfd);
  bool pfd_deinit(PFD_Data_t* pfd);
  bool pfd_load(PFD_Data_t* pfd);
  bool pfd_unload(PFD_Data_t* pfd);
  bool pfd_is_loaded(const PFD_Data_t* pfd) const;
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(RModule, 2, IModule);
};


//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
