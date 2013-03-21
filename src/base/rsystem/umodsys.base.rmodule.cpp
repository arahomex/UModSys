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
  minfo.name = RSystem::s_sys.mod_string(mi2.name);
  minfo.info = RSystem::s_sys.mod_string(mi2.info);
  minfo.verno = mi2.verno;
  return true;
}

bool RModule::scan(void)
{
  if(ireg!=NULL)
    return false;
  if(!open())
    return false;
  //
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
  return ireg!=NULL;
}

bool RModuleLibrary::lib_load(void)
{
  if(ireg!=NULL) {
    load_count++;
    return true;
  }
  if(!load0())
    return false;
  load_count++;
  return true;
}

bool RModuleLibrary::lib_unload(void)
{
  if(load_count-->0)
    return true;
  load_count = 0;
  bool rv1 = unlink();
  bool rv2 = pfd_unload(get_pfd());
  return rv1 && rv2;
}

//***************************************
//***************************************

size_t RModuleLibrary::cleanup(void)
{
  dbg_put(
    rsdl_ModuleLibrary,
    "RModuleLibrary(%p)::cleanup() { ireg=%p, load_count=%d, sys_libname=\"%s\" }\n", 
    this, ireg, load_count, sys_libname()
  );
  if(ireg==NULL)
    return 0;
  if(load_count==0) {
    unlink();
    pfd_unload(get_pfd());
  }
  return 0;
}

bool RModuleLibrary::load0(void)
{
  if(ireg!=NULL)
    return true;
  ireg = pfd_load(get_pfd(), sys_libname.get_s());
  if(ireg==NULL)
    return false;
  load_count = 0;
  if(!link()) {
    unlink();
    pfd_unload(get_pfd());
    return false;
  }
  return true;
}

bool RModuleLibrary::link(void)
{
  if(ireg==NULL)
    return false;
  for(int i=0, n=ireg->mlr_count(); i<n; i++) {
    IModuleReg *imr = ireg->mlr_get(i);
    RModule* mm = NULL;
    for(size_t k=0; k<~modules; k++) {
      if(modules(k)->minfo==imr->minfo) {
        mm = modules(k);
        break;
      }
    }
    if(mm==NULL) {
      dbg_put(rsdl_ModuleLibrary, "RModuleLibrary(%p)::link() -- add new mr\n", this);
      RModule::SelfP m = new RModule(this, imr);
      modules.push(m);
    } else {
      mm->ireg = imr;
    }
  }
  return true;
}

bool RModuleLibrary::unlink(void)
{
  if(ireg==NULL)
    return false;
  for(size_t i=0; i<~modules; i++) {
    modules(i)->ireg = NULL;
  }
  ireg->mlr_close();
  ireg = NULL;
  return true;
}

bool RModuleLibrary::scan_mr(void)
{
  dbg_put(rsdl_ModuleLibrary, "scan_mr()\n");
  if( ireg->mlr_open(&RSystem::s_sys, get_privmem()) ) {
    for(int i=0, n=ireg->mlr_count(); i<n; i++) {
      IModuleReg *imr = ireg->mlr_get(i);
      dbg_put(rsdl_ModuleLibrary, "scan_mr() i=%d imr=%p\n", i, imr);
      RModule::SelfP m = new RModule(this, imr);
      m->scan();
      modules.push(m);
    }
    ireg->mlr_close();
  }
  dbg_put(rsdl_ModuleLibrary, "/scan_mr()\n");
  return true;
}

size_t RModuleLibrary::s_find_dup(const RModuleLibraryArray& la, IModuleLibraryReg* ireg)
{
  for(size_t i=0; i<~la; i++) {
    if(la(i)->ireg==ireg)
      return i;
  }
  return array_index_none;
}

bool RModuleLibrary::s_add(RModuleLibraryArray& la, const char *filename)
{
  PFD_Raw raw_data;
  PFD_Data *pfd = reinterpret_cast<PFD_Data*>(raw_data);
  RModuleLibrary::pfd_init(pfd);
  //
  dbg_put(rsdl_ModuleLibrary, "  so: \"%s\"\n", filename);
  IModuleLibraryReg* ilib = RModuleLibrary::pfd_load(pfd, filename);
  if(ilib!=NULL) {
    dbg_put(rsdl_ModuleLibrary, "    so loaded: \"%s\"\n", filename);
    size_t dup = RModuleLibrary::s_find_dup(la, ilib);
    if(dup==array_index_none) {
      RModuleLibrary::SelfP lib = new RModuleLibrary(pfd, ilib);
      la.push(lib);
      lib->sys_libname = filename;
      lib->link();
      dbg_put(rsdl_ModuleLibrary, "    so added\n");
    } else {
      dbg_put(rsdl_ModuleLibrary, "    so dup with %d\n", (int)dup);
    }
    RModuleLibrary::pfd_unload(pfd);
    return true;
  }
  //
  RModuleLibrary::pfd_unload(pfd);
  return false;
}

//***************************************
//***************************************

RModuleLibrary::RModuleLibrary(PFD_Data* pfd, IModuleLibraryReg* imlr)
: ireg(imlr), load_count(0)
{
  pfd_init(get_pfd(), pfd);
}

RModuleLibrary::RModuleLibrary(void)
: ireg(NULL), load_count(0)
{
  pfd_init(get_pfd());
}

RModuleLibrary::~RModuleLibrary(void)
{
  unlink();
  pfd_unload(get_pfd());
}

UMODSYS_REFOBJECT_UNIIMPLEMENT_BODY(RModuleLibrary)

//***************************************
// ::
//***************************************
