#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystem::
//***************************************
/*
va_list rsystem::val(va_list va)
{
  return va;
}
*/
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
  dbg_put(
    rsdl_Uid, "RSystem::upi_add(%p:{\"%s\",\"%s\",%d})...\n", 
    lupi, lupi->group, lupi->name, lupi->verno
  );
/*
  SUniquePointerInfo* rv;
  SUniquePointerInfo nv(g, n, lupi->verno);
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
*/
  const SUniquePointerInfo* rv;
  SUniquePointerInfo nv(g, n, lupi->verno);
  //
  rv = uptr_list.find(nv);
  if(rv!=NULL)
    return rv;
  rv = uptr_list.add(nv);
  if(rv!=NULL)
    return rv;
  return NULL;
}

int RSystem::upi_remove(HUniquePointer upi)
{
  return -1;
}

//***************************************

const core::SSourceContext* RSystem::persist_ctx(const core::SSourceContext* sc)
{
  if(sc==NULL)
    return NULL;
  const DCString& ff = sc_string(sc->file);
  if(&ff==NULL)
    return NULL; // error
  const DCString& fu = sc_string(sc->function);
  if(&fu==NULL)
    return NULL; // error
  //
  const SSourceContext* rv;
  SSourceContext nv(ff, sc->line, fu);
  //
  rv = sc_list.find(nv);
  if(rv!=NULL)
    return rv;
  rv = sc_list.add(nv);
  if(rv!=NULL)
    return rv;
  return NULL;
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
  return &mema_system;
}

IMemAlloc* RSystem::get_sharemem(void)
{
  return &mema_shared;
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

ISourceContextAdapter* RSystem::get_sca(void)
{
  return this;
}

IExecutorPure* RSystem::get_executor(void)
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
