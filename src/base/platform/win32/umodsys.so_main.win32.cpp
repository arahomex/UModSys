#include "../../rsystem/umodsys.base.rsystem.h"

#include <windows.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

extern "C" __declspec(dllexport) IModuleReg* __stdcall UModSys_Plugin_Entry2(ISystem* isys, int id)
{
  return NULL;
}

int base::so_entry_enabled_flag = 1;

//***************************************
// ::
//***************************************
