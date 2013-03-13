#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RModule::
//***************************************

void RModule::ref_add(void) const {}
void RModule::ref_remove(void) const {}
int  RModule::ref_links(void) const { return 0; }

//***************************************

DCString RModule::get_sys_libname(void) const
{
  return DCString(sys_libname);
}

const SModuleInfo& RModule::get_info(void) const
{
  return minfo;
}

bool RModule::is_loaded(void) const
{
  return pfd_is_loaded(reinterpret_cast<const PFD_Data_t*>(pfd_data));
}

core::IMemAlloc* RModule::get_privmem(void) const
{
  return NULL;
}

bool RModule::load(void)
{
  return pfd_load(reinterpret_cast<PFD_Data_t*>(pfd_data));
}

bool RModule::unload(void)
{
  return pfd_unload(reinterpret_cast<PFD_Data_t*>(pfd_data));
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

RModule::RModule(void)
{
  pfd_init(reinterpret_cast<PFD_Data_t*>(pfd_data));
}

RModule::~RModule(void)
{
  pfd_deinit(reinterpret_cast<PFD_Data_t*>(pfd_data));
}


//***************************************
// ::
//***************************************
