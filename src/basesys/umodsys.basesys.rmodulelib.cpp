#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// IModuleLibraryUni::
//***************************************

DCString IModuleLibraryUni::get_sys_libname(void) const
{
  return DCString(sys_libname);
}

core::IMemAlloc* IModuleLibraryUni::get_privmem(void) const
{
  return &mema;
}

ISystem* IModuleLibraryUni::get_system(void) const
{
  return sys;
}

size_t IModuleLibraryUni::get_module_count(void) const
{
  return ~modules;
}

IModule* IModuleLibraryUni::get_module(size_t id) const
{
  return id>=~modules ? NULL : modules(id)();
}

bool IModuleLibraryUni::lib_loaded(void) const
{
  return ireg!=NULL;
}

bool IModuleLibraryUni::lib_load(void)
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

bool IModuleLibraryUni::lib_unload(void)
{
  if(load_count-->0)
    return true;
  load_count = 0;
  return true;
}

bool IModuleLibraryUni::lib_free(void)
{
  if(linked)
    return false;
  modules.Clear();
  return true;
}

size_t IModuleLibraryUni::lib_findobjname(core::IRefObject::TypeId intr, core::IRefObject::TypeId found[], size_t nfound, core::BCStr mask)
{
  size_t n = 0;
  for(size_t i=0; i<~modules; i++) {
    RModule* m = modules(i);
    if(m==NULL)
      continue;
    if(found!=NULL) {
      n += m->mod_findobjname(intr, found+n, nfound-n, mask);
    } else {
      n += m->mod_findobjname(intr, NULL, nfound, mask);
    }
  }
  return n;
}

bool IModuleLibraryUni::lib_generate(core::IRefObject::P& obj, core::IRefObject::TypeId name, const core::SParameters& args)
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

size_t IModuleLibraryUni::cleanup(void)
{
/*
  dbg_put(
    rsdl_ModuleLibrary,
    "IModuleLibraryUni(%p)::cleanup() { ireg=%p, load_count=%d, sys_libname=\"%s\" }\n", 
    this, ireg, load_count, sys_libname()
  );
*/
  if(ireg==NULL)
    return 0;
  if(load_count==0) {
    unlink();
    mema.check_leaks(true);
    uni_unload();
  }
  return 1;
}

bool IModuleLibraryUni::load0(void)
{
  if(ireg!=NULL)
    return true;
  if(!uni_load())
    return false;
  load_count = 0;
  linked = false;
  if(!link()) {
    unlink();
    mema.check_leaks(true);
    uni_unload();
    return false;
  }
  return true;
}

bool IModuleLibraryUni::link(void)
{
  if(ireg==NULL)
    return false;
  if(linked)
    return true;
  if(!uni_open())
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
      dbg_put(rsdl_ModuleLibrary, "IModuleLibraryUni(%p)::link() -- add new mr\n", this);
      RModule::SelfP m = new(M()) RModule(this, imr);
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

bool IModuleLibraryUni::unlink(void)
{
  if(ireg==NULL)
    return false;
  for(size_t i=0; i<~modules; i++) {
    modules(i)->ireg = NULL;
  }
  uni_close();
  ireg = NULL;
  linked = false;
  return true;
}

bool IModuleLibraryUni::scan(void)
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

bool IModuleLibraryUni::save_db(FILE *f)
{
#if 0
  fprintf(f, "BEGIN LIBRARY \"%s\"\n", sys_libname.get_text());
  for(size_t i=0; i<~modules; i++) {
    if(!modules(i).valid())
      continue;
    if(!modules(i)->save_db(f))
      return false;
  }
  fprintf(f, "END LIBRARY \"%s\"\n", sys_libname.get_text());
#else
  fprintf(f, "LIBRARY \"%s\" {\n", sys_libname.get_text());
  for(size_t i=0; i<~modules; i++) {
    if(!modules(i).valid())
      continue;
    if(!modules(i)->save_db(f))
      return false;
  }
  fprintf(f, "} # END LIBRARY \"%s\"\n\n", sys_libname.get_text());
#endif
  return true;
}

//***************************************
//***************************************

IModuleLibraryUni::IModuleLibraryUni(ISystem* s, IModuleLibraryReg* imlr = NULL)
: ireg(imlr), load_count(0), linked(false), sys(s), mema(NULL)
{
}

IModuleLibraryUni::~IModuleLibraryUni(void)
{
}

//***************************************
// ::
//***************************************
