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
  dbg_put(
    "RModule::alloc_minfo({\"%s\", \"%s\", {%d,%d} })\n", 
    mi2.name(), mi2.info(), mi2.verno.vmajor, mi2.verno.vminor
  );
  minfo.name = RSystem::s_sys.mod_string(mi2.name);
  minfo.info = RSystem::s_sys.mod_string(mi2.info);
  minfo.verno = mi2.verno;
  //
  //
  return true;
}

//***************************************
//***************************************

RModule::RModule(RModuleLibrary *pv, IModuleReg *imr)
: owner(NULL), ireg(imr)
{
  rc_init(pv);
  if(imr!=NULL) {
    alloc_minfo(imr->minfo);
  }
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

bool RModuleLibrary::lib_loaded(void) const
{
  return pfd_is_loaded(reinterpret_cast<const PFD_Data*>(pfd_data));
}

bool RModuleLibrary::lib_load(void)
{
  return pfd_load(reinterpret_cast<PFD_Data*>(pfd_data), sys_libname.get_s());
}

bool RModuleLibrary::lib_unload(void)
{
  return pfd_unload(reinterpret_cast<PFD_Data*>(pfd_data));
}

//***************************************
//***************************************

bool RModuleLibrary::scan_mr(PFD_Data* pfd)
{
  dbg_put("scan_mr()\n");
  for(int i=0; ; i++) {
    IModuleReg *imr = pfd_getmr(pfd, i);
    if(imr==NULL)
      break;
    dbg_put("scan_mr() i=%d imr=%p\n", i, imr);
    modules.push(new RModule(this, imr));
  }
  dbg_put("/scan_mr()\n");
  return true;
}

//***************************************
//***************************************

RModuleLibrary::RModuleLibrary(PFD_Data* pfd)
{
  pfd_init(reinterpret_cast<PFD_Data*>(pfd_data), pfd);
  scan_mr(reinterpret_cast<PFD_Data*>(pfd_data));
}

RModuleLibrary::RModuleLibrary(void)
{
  pfd_init(reinterpret_cast<PFD_Data*>(pfd_data));
}

RModuleLibrary::~RModuleLibrary(void)
{
  pfd_deinit(reinterpret_cast<PFD_Data*>(pfd_data));
}

UMODSYS_REFOBJECT_UNIIMPLEMENT_BODY(RModuleLibrary)

//***************************************
// ::
//***************************************
