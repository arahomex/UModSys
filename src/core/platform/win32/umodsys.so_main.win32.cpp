#include <umodsys/core/module_impl.h>

#include <windows.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

extern "C" __declspec(dllexport) IModuleLibraryReg* __stdcall UModSys_Plugin_Entry2(void)
{
  return &RModuleLibraryReg_Chain::s_library;
}

int base::so_entry_enabled_flag = 1;

//***************************************
// ::
//***************************************
