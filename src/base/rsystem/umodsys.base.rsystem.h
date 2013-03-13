#ifndef __UMODSYS_BASE_RSYSTEM_H
#define __UMODSYS_BASE_RSYSTEM_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rsystem.h
// info: implementation of base system object
/*************************************************************/

#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>

namespace UModSys {
namespace base {

using namespace core;

//***************************************
// ISystem
//***************************************

struct RSystem : public ISystem 
{
public:
  RSystem(void);
  ~RSystem(void);
public:
  void ref_add(void) const;
  void ref_remove(void) const;
  int  ref_links(void) const;
public:
  SUniquePointerInfo* upi_add(const SUniquePointerInfo* lupi);
  int upi_remove(SUniquePointerInfo* upi);
public:
  IConsole* get_console(void);
  IMemAlloc* get_sysmem(void);
  IMemAlloc* get_sharemem(void);
public:
  IModule* module_preload(const DCString& sys_name);
  IModule* module_load(const DCString& name, const SVersion& verno, bool doload);
  bool module_unload(const DCString& name, const SVersion& verno);
  bool module_unload(IModule* M);
public:
  IConsole* console;
  //
  bool init(void);
  bool exec_args(int argc, char** argv);
  bool exec_main(void);
  bool deinit(void);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(RSystem, 2, ISystem);
public:
  static RSystem s_sys;
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RSYSTEM_H
