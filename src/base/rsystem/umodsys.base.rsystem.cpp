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
  //
  SUniquePointerInfo* rv;
  SUniquePointerInfo nv(g, n, lupi->verno);
//  dbg_put(
//    rsdl_System, "RSystem::upi_add(%p:{\"%s\",\"%s\",%d})...\n", 
//    lupi, lupi->group, lupi->name, lupi->verno
//  );
  {
    int idx = 0;
    SUniPtrHolder *h, *lh = NULL;
    if(uptr_list.first(idx, h)) {
      do {
        for(size_t i=0; i<h->used; i++) {
          const SUniquePointerInfo& x = h->elems[i];
          if(x == nv)
            return &x;
        }
        lh = h;
      } while(uptr_list.next(idx, h));
    }
    if(lh==NULL || lh->used>=upi_quant) {
      idx++;
      lh = uptr_list[idx];
      if(lh==NULL) {
        return NULL; // ERROR
      }
    }
    rv = &lh->elems[lh->used++];
  }
  //
  *rv = nv;
//  dbg_put(
//    rsdl_System, "RSystem::upi_add(%p:{\"%s\",\"%s\",%d}) => %p\n", 
//    lupi, lupi->group, lupi->name, lupi->verno, rv
//  );
  return rv;
}

int RSystem::upi_remove(HUniquePointer upi)
{
  return -1;
}

//***************************************

IParameters* RSystem::get_params(void)
{
  return params;
}

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
  return &moddb;
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


//***************************************
// ::
//***************************************
