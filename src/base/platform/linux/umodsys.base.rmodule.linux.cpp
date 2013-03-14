#include "../../rsystem/umodsys.base.rsystem.h"
#include "../../rsystem/umodsys.base.rmodule.h"

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <dlfcn.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

#define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"

typedef IModuleReg* (*f_get_moduleinfo)(ISystem* isys, int id);

struct RModuleLibrary::PFD_Data {
  void* module;
  f_get_moduleinfo entry;
};

bool RModuleLibrary::pfd_init(PFD_Data* pfd)
{
  pfd->module = NULL;
  pfd->entry = NULL;
  return true;
}

bool RModuleLibrary::pfd_deinit(PFD_Data* pfd)
{
  pfd_unload(pfd);
  return true;
}

bool RModuleLibrary::pfd_load(PFD_Data* pfd)
{
  if(pfd->module!=NULL)
    return false;
//  SetDllDirectory();
  pfd->module = dlopen(sys_libname, RTLD_LOCAL|RTLD_NOW);
  if(pfd->module==NULL)
    return false;
  //
  pfd->entry = (f_get_moduleinfo)(dlsym(pfd->module, MODULE_ENTRY_NAME));
  if(pfd->entry==NULL) {
    pfd_unload(pfd);
    return false;
  }
  //
/*
  pfd->ireg = pfd->entry(&RSystem::s_sys, sys_id);
  if(pfd->ireg==NULL) {
    pfd_unload(pfd);
    return false;
  }
*/
  //
  return true;
}

bool RModuleLibrary::pfd_unload(PFD_Data* pfd)
{
  if(pfd->module!=NULL) {
    dlclose(pfd->module);
    pfd->module = NULL;
  }
  pfd->entry = NULL;
  return true;
}

bool RModuleLibrary::pfd_is_loaded(const PFD_Data* pfd) const
{
  return pfd->module!=NULL;
}

//***************************************
// ::
//***************************************
