#include "umodsys.base.rsystem.h"
#include "../system/umodsys.base.rsystem.h"

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

const DCString& RModule::get_sys_libname(void) const
{
  return sys_libname;
}

const SModuleInfo& get_info(void) const
{
  return minfo;
}

bool RModule::is_loaded(void) const
{
  return pfd_is_loaded(static_cast<const PFD_Data_t*>(pfd_data));
}

core::IMemAlloc* RModule::get_privmem(void) const
{
  return NULL;
}

bool RModule::load(void)
{
  return pfd_load(static_cast<PFD_Data_t*>(pfd_data));
}

bool RModule::unload(void)
{
  return pfd_unload(static_cast<PFD_Data_t*>(pfd_data));
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
  pfd_init(static_cast<PFD_Data_t*>(pfd_data));
}

RModule::~RModule(void)
{
  pfd_deinit(static_cast<PFD_Data_t*>(pfd_data));
}


//***************************************
// ::
//***************************************
