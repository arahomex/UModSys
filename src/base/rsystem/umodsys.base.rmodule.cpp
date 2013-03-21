#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

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

//***************************************

bool RModule::is_open(void) const
{
  return ireg!=NULL && ireg->mr_isopen();
}

bool RModule::open(void)
{
  dbg_put(rsdl_Module, "RModule(%p)::open()\n", this);
  if(ireg!=NULL)
    return ireg->mr_open();
  if(!owner->lib_load())
    return false;
  if(ireg!=NULL && ireg->mr_open())
    return true;
  owner->lib_unload();
  return false;
}

bool RModule::close(void)
{
  dbg_put(rsdl_Module, "RModule(%p)::close()\n", this);
  if(ireg==NULL)
    return false;
  ireg->mr_close();
  if(!ireg->mr_isopen())
    owner->lib_unload();
  return true;
}

//***************************************

bool RModule::reg(IModObject* mo, bool doreg)
{
  return true;
}

bool RModule::reg(IGenerator* gen, bool doreg)
{
  return true;
}

//***************************************
//***************************************

bool RModule::alloc_minfo(const SModuleInfo &mi2)
{
  dbg_put(
    rsdl_Module, "RModule::alloc_minfo({\"%s\", \"%s\", {%d,%d} })\n", 
    mi2.name(), mi2.info(), mi2.verno.vmajor, mi2.verno.vminor
  );
  minfo.name = owner->sys->get_modloader()->moduledb_get_string(mi2.name);
  minfo.info = owner->sys->get_modloader()->moduledb_get_string(mi2.info);
  minfo.verno = mi2.verno;
  return true;
}

bool RModule::scan(void)
{
  if(ireg==NULL)
    return false;
  //
  dbg_put(rsdl_Module, "RModule(%p)::open()\n", this);
  if(!open())
    return false;
  //
  // -- scan complete
  //
  close();
  return true;
}

//***************************************
//***************************************

RModule::RModule(RModuleLibrary *pv, IModuleReg *imr)
: owner(NULL), ireg(imr)
{
  rc_init(pv);
  if(ireg!=NULL) {
    alloc_minfo(ireg->minfo);
    ireg->module = this;
  }
}

RModule::~RModule(void)
{
  if(ireg!=NULL) {
    ireg->module = NULL;
    ireg = NULL;
  }
}

UMODSYS_REFOBJECT_UNIIMPLEMENT_BODY(RModule)

//***************************************
// ::
//***************************************
