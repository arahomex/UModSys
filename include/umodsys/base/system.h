#ifndef __UMODSYS_BASE_SYSTEM_H
#define __UMODSYS_BASE_SYSTEM_H 1
/*************************************************************/
// file: umodsys/base/system.h
// info: base system object
/*************************************************************/

#include <umodsys/core/stdcore.h>

#include <umodsys/base/console.h>
#include <umodsys/base/module.h>

namespace UModSys {
namespace base {

//***************************************
// ISystem
//***************************************

struct ISystem : public core::IRoot
{
public:
  ~ISystem(void);
public:
  virtual IModuleLoader* get_modloader(void) =0;
  virtual core::IUniquePointerResolver* get_upr(void) =0;
  virtual IModuleLibrary* get_syslib(void) = 0;
  //
  virtual core::IMemAlloc* get_sysmem(void) =0;
  virtual core::IMemAlloc* get_sharemem(void) =0;
  virtual IConsole* get_console(void) =0;
public:
  UMODSYS_ROOT_IMPLEMENT1(base::ISystem, 2, IRoot);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_SYSTEM_H
