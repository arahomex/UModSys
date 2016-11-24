#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystemKernel::
//***************************************
/*
va_list rsystem::val(va_list va)
{
  return va;
}
*/
//***************************************
// RSystemKernel::
//***************************************

//void RSystemKernel::ref_add(void) const {}
//void RSystemKernel::ref_remove(void) const {}
//int  RSystemKernel::ref_links(void) const { return 0; }

//***************************************

HUniquePointer RSystemKernel::upi_add(const SUniquePointerInfo* lupi)
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
    rsdl_Uid, "RSystemKernel::upi_add(%p:{\"%s\",\"%s\",%d})...\n", 
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
//    rsdl_System, "RSystemKernel::upi_add(%p:{\"%s\",\"%s\",%d}) => %p\n", 
//    lupi, lupi->group, lupi->name, lupi->verno, rv
//  );
  return rv;
*/
  const SUniquePointerInfo* rv;
  SUniquePointerInfo nv(*g, *n, lupi->verno);
  //
  rv = uptr_list.find(nv);
  if(rv!=NULL)
    return rv;
  rv = uptr_list.add(nv);
  if(rv!=NULL)
    return rv;
  return NULL;
}

int RSystemKernel::upi_remove(HUniquePointer upi)
{
  return -1;
}

//***************************************

const core::SSourceContext* RSystemKernel::persist_ctx(const core::SSourceContext* sc)
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
  SSourceContext nv(*ff, sc->line, *fu);
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

IParameters* RSystemKernel::get_params(void)
{
  return params;
}

IConsole* RSystemKernel::get_console(void)
{
  return console;
}

IMemAlloc* RSystemKernel::get_sysmem(void)
{
  return &mema_system;
}

IMemAlloc* RSystemKernel::get_sharemem(void)
{
  return &mema_shared;
}

IModuleLibrary* RSystemKernel::get_syslib(void)
{
  return NULL;
}

IModuleLoader* RSystemKernel::get_modloader(void)
{
  return &moddb;
}

IUniquePointerResolver* RSystemKernel::get_upr(void)
{
  return this;
}

ISourceContextAdapter* RSystemKernel::get_sca(void)
{
  return this;
}

IExecutorPure* RSystemKernel::get_executor(void)
{
  return this;
}

//***************************************

#if 0
core::DCString RSystemKernel::get_sys_libname(void) const
{
  return DCString("system");
}

core::IMemAlloc* RSystemKernel::get_privmem(void) const
{
  return get_sharedmem();
}

size_t RSystemKernel::get_module_count(void) const
{
  return 0;
}

IModule* RSystemKernel::get_module(size_t id) const
{
  return NULL;
}

bool RSystemKernel::lib_loaded(void) const
{
  return true;
}

bool RSystemKernel::lib_load(void)
{
  return false;
}

bool RSystemKernel::lib_unload(void)
{
  return false;
}
#endif

//***************************************


//***************************************
// ::
//***************************************
