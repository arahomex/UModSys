#ifndef __UMODSYS_BASE_TYPES_H
#define __UMODSYS_BASE_TYPES_H 1
/*************************************************************/
// file: umodsys/base/basetypes.h
// info: base types & forwareds
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/tl/util/static_holder.h>


namespace UModSys {
namespace base {

//***************************************
// USING

using core::SParameters;
using core::DCString;
using core::BCStr;
using core::IRefObject;

//***************************************
// Forwards

// system.h
struct ISystem;
struct SModuleContext;

// console.h
struct IConsole;
struct SDebug;

// module.h
struct IModule;
struct IModuleLibrary;
struct IModuleLoader;
struct SModuleInfo;

// shell.h
struct IShell;
struct IExecutorPure;
struct IExecutor;

// bobjects.h
struct IModObject;
struct IGenerator;

// module_impl.h
struct IModuleReg;
struct IModuleLibraryReg;
struct SModuleRegChain;
struct RModuleLibraryReg_Chain;


//***************************************
// eConsoleLevels
//***************************************

enum eConsoleLevels {
  cl_Fatal     = 0,
  cl_Error     = 1,
  cl_Warning   = 2,
  cl_UI        = 3,
  cl_Info      = 4,
  cl_Aux       = 5,
  cl_Debug     = 6,
  cl_Debug1    = 7,
  cl_Debug2    = 8,
  cl_Last      = 0x7fff
};

//***************************************
// END
//***************************************

} // namespace base

//***************************************
// 
//***************************************

extern base::SModuleContext M;

//***************************************
// END
//***************************************

} // namespace UModSys

#endif // __UMODSYS_BASE_TYPES_H
