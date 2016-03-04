#ifndef __UMODSYS_BASE_RDEBUG_H
#define __UMODSYS_BASE_RDEBUG_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rcommon.h
// info: common constants and variables
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/core/syshlp.h>
#include <umodsys/core/stdstrings.h>

#include <umodsys/tl/container/dynarray.h>
#include <umodsys/tl/container/scatter_array.h>
#include <umodsys/tl/container/quant_array.h>

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

struct IModuleLibraryUni;

struct RModuleLibrarySO;
struct RModuleLibraryThis;

struct RModule;
struct RConsole_std;

typedef tl::TDynarrayDynamic<
  tl::TRefObject<RModuleLibrarySO>,
  tl::DAllocatorMallocFast
> RModuleLibrarySOArray;

typedef tl::TStaticStringPool<
  core::BChar,
  tl::su::TComparerBinaryHash<core::BChar>,
  core::SMemAlloc_Malloc
> DSystemStaticPool;

//***************************************
// DEBUG CONST
//***************************************

enum eRSystemDebugLevels {
  rsdl_MemoryError,
  rsdl_System,
  rsdl_Module,
  rsdl_ModuleLibrary,
  rsdl_SoLoad,
  rsdl_Uid,
  rsdl_SysTests,
  rsdl_TCL,
};

extern SDebug rsys_dbg;

void dbg_put(eRSystemDebugLevels dl, const char *fmt, ...);
void err_put(eRSystemDebugLevels dl, const char *fmt, ...);

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
