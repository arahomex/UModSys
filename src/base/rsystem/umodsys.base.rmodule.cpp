#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

IModuleLibrary* RModule::get_library(void) const
{
  return owner;
}

const SModuleInfo& RModule::get_info(void) const
{
  return minfo;
}

bool RModule::is_open(void) const
{
  return ireg!=NULL;
}

bool RModule::open(void)
{
  return false;
}

bool RModule::close(void)
{
  return false;
}

//***************************************
//***************************************

bool RModule::alloc_minfo(const SModuleInfo &mi2)
{
  mi_name = mi2.name;
  mi_info = mi2.info;
  minfo.name = mi_name;
  minfo.info = mi_info;
  minfo.verno = mi2.verno;
  return true;
}

//***************************************
//***************************************

RModule::RModule(RModuleLibrary *pv)
: owner(NULL), ireg(NULL) 
{
  rc_init(pv);
}

RModule::~RModule(void)
{
}

UMODSYS_REFOBJECT_UNIIMPLEMENT_BODY(RModule)

//***************************************
// RModuleLibrary::
//***************************************

DCString RModuleLibrary::get_sys_libname(void) const
{
  return DCString(sys_libname);
}

core::IMemAlloc* RModuleLibrary::get_privmem(void) const
{
  return NULL;
}

size_t RModuleLibrary::get_module_count(void) const
{
  return ~modules;
}

IModule* RModuleLibrary::get_module(size_t id) const
{
  return id>=~modules ? NULL : modules(id)();
}

bool RModuleLibrary::is_loaded(void) const
{
  return pfd_is_loaded(reinterpret_cast<const PFD_Data*>(pfd_data));
}

bool RModuleLibrary::load(void)
{
  return pfd_load(reinterpret_cast<PFD_Data*>(pfd_data));
}

bool RModuleLibrary::unload(void)
{
  return pfd_unload(reinterpret_cast<PFD_Data*>(pfd_data));
}

//***************************************
//***************************************

//***************************************
//***************************************

RModuleLibrary::RModuleLibrary(void)
{
  pfd_init(reinterpret_cast<PFD_Data*>(pfd_data));
}

RModuleLibrary::~RModuleLibrary(void)
{
  pfd_deinit(reinterpret_cast<PFD_Data*>(pfd_data));
}

//***************************************
// ::
//***************************************
