#include "../../../rsystem/umodsys.base.rsystem.h"
#include "../../../rsystem/umodsys.base.rmodule.h"

#include <windows.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

#if defined(_M_X64) || defined(_M_IA64)
  #define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"
#else
  #define MODULE_ENTRY_NAME "_UModSys_Plugin_Entry2@8"
#endif

typedef IModuleReg* (__stdcall *f_get_moduleinfo)(ISystem* isys, int id);

struct RModule::PFD_Data_t {
  HMODULE module;
  f_get_moduleinfo entry;
  IModuleReg* ireg;
};

bool RModule::pfd_init(PFD_Data_t* pfd)
{
  pfd->module = NULL;
  pfd->entry = NULL;
  pfd->ireg = NULL;
  return true;
}

bool RModule::pfd_deinit(PFD_Data_t* pfd)
{
  pfd_unload(pfd);
  return true;
}

bool RModule::pfd_load(PFD_Data_t* pfd)
{
  if(pfd->module!=NULL)
    return false;
//  SetDllDirectory();
  pfd->module = LoadLibraryEx(
    sys_libname, 
    NULL, 
    0
  );
  if(pfd->module==NULL)
    return false;
  //
  pfd->entry = (f_get_moduleinfo)(GetProcAddress(pfd->module, MODULE_ENTRY_NAME));
  if(pfd->entry==NULL) {
    pfd_unload(pfd);
    return false;
  }
  //
  pfd->ireg = pfd->entry(&RSystem::s_sys, sys_id);
  if(pfd->ireg==NULL) {
    pfd_unload(pfd);
    return false;
  }
  //
  return true;
}

bool RModule::pfd_unload(PFD_Data_t* pfd)
{
  if(pfd->module!=NULL) {
    FreeLibrary(pfd->module);
    pfd->module = NULL;
  }
  pfd->ireg = NULL;
  pfd->entry = NULL;
  return true;
}

bool RModule::pfd_is_loaded(const PFD_Data_t* pfd) const
{
  return pfd->module!=NULL;
}

//***************************************
// ::
//***************************************
