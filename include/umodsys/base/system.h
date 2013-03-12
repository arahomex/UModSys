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

struct ISystem 
: public core::IRefObject, 
  public core::IUniquePointerResolver,
  public IModuleLoader,
{
public:
  ~ISystem(void);
public:
  virtual IConsole* get_console(void) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(ISystem, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_SYSTEM_H
