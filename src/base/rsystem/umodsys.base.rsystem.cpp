#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystem::
//***************************************

//void RSystem::ref_add(void) const {}
//void RSystem::ref_remove(void) const {}
//int  RSystem::ref_links(void) const { return 0; }

//***************************************

HUniquePointer RSystem::upi_add(const SUniquePointerInfo* lupi)
{
  if(lupi==NULL)
    return NULL; // error
  const DCString& g = uptr_string(lupi->group);
  if(&g==NULL)
    return NULL; // error
  const DCString& n = uptr_string(lupi->name);
  if(&n==NULL)
    return NULL; // error
  SUniquePointerInfo nv(g, n, lupi->verno);
  for(int i=0; i<~uptr_list; i++) {
    const SUniquePointerInfo& x = uptr_list(i);
    if(x == nv)
      return &x;
  }
  if(!uptr_list.push(nv))
    return NULL;
  return &uptr_list.last();
}

int RSystem::upi_remove(HUniquePointer upi)
{
  return -1;
}

//***************************************

IConsole* RSystem::get_console(void)
{
  return console;
}

IMemAlloc* RSystem::get_sysmem(void)
{
  return NULL;
}

IMemAlloc* RSystem::get_sharemem(void)
{
  return NULL;
}

IModuleLibrary* RSystem::get_syslib(void)
{
  return NULL;
}

IModuleLoader* RSystem::get_modloader(void)
{
  return this;
}

IUniquePointerResolver* RSystem::get_upr(void)
{
  return this;
}

//***************************************

#if 0
core::DCString RSystem::get_sys_libname(void) const
{
  return DCString("system");
}

core::IMemAlloc* RSystem::get_privmem(void) const
{
  return get_sharedmem();
}

size_t RSystem::get_module_count(void) const
{
  return 0;
}

IModule* RSystem::get_module(size_t id) const
{
  return NULL;
}

bool RSystem::lib_loaded(void) const
{
  return true;
}

bool RSystem::lib_load(void)
{
  return false;
}

bool RSystem::lib_unload(void)
{
  return false;
}
#endif

//***************************************

const DCString& RSystem::moduledb_get_string(const DCString &s)
{
  return mod_string(s);
}

size_t RSystem::moduledb_lib_count(void)
{
  return ~mod_list;
}

IModuleLibrary* RSystem::moduledb_lib_get(size_t id) const
{
  return id<~mod_list ? mod_list(id)() : NULL;
}

bool RSystem::moduledb_lib_drop(IModuleLibrary* lib)
{
  return false;
}

size_t RSystem::moduledb_module_count(void)
{
  size_t n = 0;
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    n += ~ml->modules;
  }
  return n;
}

IModule* RSystem::moduledb_module_get(size_t id) const
{
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    if(id<~ml->modules) {
      return ml->modules(id);
    }
    id -= ~ml->modules;
  }
  return NULL;
}

IModule* RSystem::moduledb_find(const core::DCString& name, const core::SVersion& verno) const
{
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    for(size_t k=0; k<~ml->modules; k++) {
      RModule* m = ml->modules(k);
      if(m==NULL)
        continue;
      if(m->minfo.eq(name, verno))
        return m;
    }
  }
  return NULL;
}

void RSystem::moduledb_clear(void)
{
  moduledb_cleanup();
  mod_list.clear();
}

size_t RSystem::moduledb_cleanup(void)
{
  size_t n, rv = 0;
  do {
    n=0;
    for(int i=0; i<~mod_list; i++) {
      size_t nn = mod_list(i)->cleanup();
      if(nn==0)
        continue;
      n += nn;
    }
    rv += n;
  } while(n);
  return rv;
}

bool RSystem::moduledb_load(const core::DCString& cachepath)
{
  return false;
}

bool RSystem::moduledb_save(const core::DCString& cachepath)
{
  return false;
}

size_t RSystem::moduledb_scan(const core::DCString& mask)
{
  for(size_t i=0; i<~mod_list; i++) {
    mod_list(i)->load0();
  }
  bool rv = RModuleLibrary::pfd_scan(this, mod_list, mask);
  moduledb_cleanup();
  return rv;
}


//***************************************
// ::
//***************************************
