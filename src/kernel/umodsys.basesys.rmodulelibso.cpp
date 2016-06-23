#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RModuleLibrarySO::
//***************************************
  //
bool RModuleLibrarySO::uni_load(void)
{
  ireg = pfd_load(get_pfd(), sys_libname.get_s());
  if(ireg==NULL)
    return false;
  return true;
}

bool RModuleLibrarySO::uni_unload(void)
{
  return pfd_unload(get_pfd());
}

bool RModuleLibrarySO::uni_open(void)
{
  return ireg!=NULL ? ireg->mlr_open(sys, get_privmem()) : false;
}

bool RModuleLibrarySO::uni_close(void)
{
  return ireg!=NULL ? ireg->mlr_close() : false;
}

//***************************************
//***************************************

size_t RModuleLibrarySO::s_find_dup(const RModuleLibrarySOArray& la, IModuleLibraryReg* ireg)
{
  for(size_t i=0; i<~la; i++) {
    if(la(i)->ireg==ireg)
      return i;
  }
  return array_index_none;
}

bool RModuleLibrarySO::s_add(ISystem* sys, RModuleLibrarySOArray& la, const char *filename)
{
  PFD_Raw raw_data;
  PFD_Data *pfd = reinterpret_cast<PFD_Data*>(raw_data);
  RModuleLibrarySO::pfd_init(pfd);
  //
  dbg_put(rsdl_ModuleLibrary, "  so: \"%s\"\n", filename);
  IModuleLibraryReg* ilib = RModuleLibrarySO::pfd_load(pfd, filename);
  if(ilib!=NULL) {
    dbg_put(rsdl_ModuleLibrary, "    so loaded: \"%s\"\n", filename);
    size_t dup = RModuleLibrarySO::s_find_dup(la, ilib);
    if(dup==array_index_none) {
      RModuleLibrarySO::SelfP lib = new(M()) RModuleLibrarySO(sys, pfd, ilib);
      la.Push(lib);
      lib->sys_libname = filename;
      lib->link();
      lib->scan();
      dbg_put(rsdl_ModuleLibrary, "    so added\n");
    } else {
      dbg_put(rsdl_ModuleLibrary, "    so dup with %d\n", (int)dup);
    }
    RModuleLibrarySO::pfd_unload(pfd);
    return true;
  }
  //
  RModuleLibrarySO::pfd_unload(pfd);
  return false;
}

//***************************************
//***************************************

RModuleLibrarySO::RModuleLibrarySO(ISystem* s, PFD_Data* pfd, IModuleLibraryReg* imlr)
: IModuleLibraryUni(s, imlr)
{
  pfd_init(get_pfd(), pfd);
}

RModuleLibrarySO::RModuleLibrarySO(ISystem* s)
: IModuleLibraryUni(s, NULL)
{
  pfd_init(get_pfd());
}

RModuleLibrarySO::~RModuleLibrarySO(void)
{
  unlink();
  pfd_unload(get_pfd());
}

//***************************************
// ::
//***************************************
