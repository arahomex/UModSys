#include "../../rsystem/umodsys.base.rsystem.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

#ifdef UMODSYS_ENABLE_DLLEXPORT
  #define MODULE_EXPORT __declspec(dllexport)
#else
  #define MODULE_EXPORT __attribute__((visibility("default")))
#endif

extern "C" MODULE_EXPORT IModuleReg* UModSys_Plugin_Entry2(ISystem* isys, int id)
{
  M.Link(isys, NULL);
  return SModuleRegChain::s_get(id);
}

int base::so_entry_enabled_flag = 1;

//***************************************
// ::
//***************************************
