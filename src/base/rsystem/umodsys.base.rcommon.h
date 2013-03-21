#ifndef __UMODSYS_BASE_RDEBUG_H
#define __UMODSYS_BASE_RDEBUG_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rcommon.h
// info: common constants and variables
/*************************************************************/

#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>

#include <umodsys/core/syshlp.h>

#include <umodsys/tl/composite/array.h>

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

struct RModuleLibrary;
struct RModule;
struct RConsole_std;


typedef tl::TStaticPool<
  core::BChar,
  tl::su::TComparerBinaryHash<core::BChar>,
  core::SMemAlloc_Malloc
> DSystemStaticPool;

typedef tl::TArray< tl::TRefObject<RModuleLibrary> > RModuleLibraryArray;

//***************************************
// DEBUG CONST
//***************************************

enum eRSystemDebugLevels {
  rsdl_System,
  rsdl_Module,
  rsdl_ModuleLibrary,
  rsdl_SoLoad,
};

extern SDebug rsys_dbg;

void dbg_put(eRSystemDebugLevels dl, const char *fmt, ...);

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
