#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RModuleLibrary::
//***************************************

DCString RModuleLibrary::get_sys_libname(void) const
{
  return DCString(sys_libname);
}

core::IMemAlloc* RModuleLibrary::get_privmem(void) const
{
  return &mema;
}

ISystem* RModuleLibrary::get_system(void) const
{
  return sys;
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
/*
  bool rv1 = unlink();
  bool rv2 = pfd_unload(get_pfd());
  return rv1 && rv2;
*/
  return true;
}

size_t RModuleLibrary::lib_findobjname(core::IRefObject::TypeId intr, core::IRefObject::TypeId found[], size_t nfound)
{
  size_t n = 0;
  for(size_t i=0; i<~modules; i++) {
    RModule* m = modules(i);
    if(m==NULL)
      continue;
    if(found!=NULL) {
      n += m->mod_findobjname(intr, found+n, nfound-n);
    } else {
      n += m->mod_findobjname(intr, NULL, nfound);
    }
  }
  return n;
}

bool RModuleLibrary::lib_generate(core::IRefObject::P& obj, core::IRefObject::TypeId name, const core::SParameters& args)
{
  for(size_t i=0; i<~modules; i++) {
    RModule* m = modules(i);
    if(m==NULL)
      continue;
    if(m->mod_generate(obj, name, args))
      return true;
  }
  return false;
}

//***************************************
//***************************************

size_t RModuleLibrary::cleanup(void)
{
/*
  dbg_put(
    rsdl_ModuleLibrary,
    "RModuleLibrary(%p)::cleanup() { ireg=%p, load_count=%d, sys_libname=\"%s\" }\n", 
    this, ireg, load_count, sys_libname()
  );
*/
  if(ireg==NULL)
    return 0;
  if(load_count==0) {
    unlink();
    pfd_unload(get_pfd());
  }
  return 1;
}

bool RModuleLibrary::load0(void)
{
  if(ireg!=NULL)
    return true;
  ireg = pfd_load(get_pfd(), sys_libname.get_s());
  if(ireg==NULL)
    return false;
  load_count = 0;
  linked = false;
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
  if(linked)
    return true;
  if(!ireg->mlr_open(sys, get_privmem()))
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
      modules.Push(m);
    } else {
      mm->ireg = imr;
      imr->module = mm;
    }
  }
  for(size_t i=0; i<~modules; i++) {
    if(modules(i)->ireg==NULL) {
      return false; // bad library, not all modules linked
    }
  }
  linked = true;
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
  linked = false;
  return true;
}

bool RModuleLibrary::scan(void)
{
  if(ireg==NULL)
    return false;
  if(!linked && !link())
    return false;
  for(size_t i=0; i<~modules; i++) {
    modules(i)->scan();
  }
  return true;
}

bool RModuleLibrary::save_db(FILE *f)
{
#if 0
  fprintf(f, "BEGIN LIBRARY \"%s\"\n", sys_libname());
  for(size_t i=0; i<~modules; i++) {
    if(!modules(i).valid())
      continue;
    if(!modules(i)->save_db(f))
      return false;
  }
  fprintf(f, "END LIBRARY \"%s\"\n", sys_libname());
#else
  fprintf(f, "LIBRARY \"%s\" {\n", sys_libname());
  for(size_t i=0; i<~modules; i++) {
    if(!modules(i).valid())
      continue;
    if(!modules(i)->save_db(f))
      return false;
  }
  fprintf(f, "} # END LIBRARY \"%s\"\n\n", sys_libname());
#endif
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

bool RModuleLibrary::s_add(ISystem* sys, RModuleLibraryArray& la, const char *filename)
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
      RModuleLibrary::SelfP lib = new RModuleLibrary(sys, pfd, ilib);
      la.Push(lib);
      lib->sys_libname = filename;
      lib->link();
      lib->scan();
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

RModuleLibrary::RModuleLibrary(ISystem* s, PFD_Data* pfd, IModuleLibraryReg* imlr)
: ireg(imlr), load_count(0), linked(false), sys(s), mema(NULL)
{
  pfd_init(get_pfd(), pfd);
}

RModuleLibrary::RModuleLibrary(void)
: ireg(NULL), load_count(0), linked(false), sys(NULL), mema(NULL)
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
