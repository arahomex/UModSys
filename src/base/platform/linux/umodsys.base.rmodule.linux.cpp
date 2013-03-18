#include "../../rsystem/umodsys.base.rsystem.h"
#include "../../rsystem/umodsys.base.rmodule.h"

//#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <dlfcn.h>
#include <dirent.h> 

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

#if defined(UMODSYS_ENABLE_DLLEXPORT) || defined(WIN32)
  #define SO_SUFFIX ".dll"
#else
  #define SO_SUFFIX ".so"
#endif

#define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"

typedef IModuleReg* (*f_get_moduleinfo)(ISystem* isys, int id);

//***************************************
// RModule::
//***************************************

struct RModuleLibrary::PFD_Data {
  void* module;
  f_get_moduleinfo entry;
};

IModuleReg* RModuleLibrary::pfd_getmr(PFD_Data* pfd, int id)
{
  dbg_put("pfd {m=%p, e=%p}\n", pfd->module, pfd->entry);
  if(pfd->module==NULL || pfd->entry==NULL)
    return NULL;
  return pfd->entry(&RSystem::s_sys, id);
}

bool RModuleLibrary::pfd_init(PFD_Data* pfd)
{
  pfd->module = NULL;
  pfd->entry = NULL;
  return true;
}

bool RModuleLibrary::pfd_init(PFD_Data* pfd, PFD_Data* pfdR)
{
  *pfd = *pfdR;
  pfd_init(pfdR);
  return true;
}

bool RModuleLibrary::pfd_deinit(PFD_Data* pfd)
{
  pfd_unload(pfd);
  return true;
}

bool RModuleLibrary::pfd_load(PFD_Data* pfd, const core::DCString& filename)
{
  if(pfd->module!=NULL)
    return false;
//  SetDllDirectory();
  pfd->module = dlopen(filename, RTLD_LOCAL|RTLD_NOW);
  if(pfd->module==NULL)
    return false;
  //
  pfd->entry = (f_get_moduleinfo)(dlsym(pfd->module, MODULE_ENTRY_NAME));
  if(pfd->entry==NULL) {
    pfd_unload(pfd);
    return false;
  }
  //
  return true;
}

bool RModuleLibrary::pfd_unload(PFD_Data* pfd)
{
  if(pfd->module!=NULL) {
    dlclose(pfd->module);
    pfd->module = NULL;
  }
  pfd->entry = NULL;
  return true;
}

bool RModuleLibrary::pfd_is_loaded(const PFD_Data* pfd) 
{
  return pfd->module!=NULL;
}

//***************************************
//***************************************

static size_t s_pfd_scan(RModuleLibraryArray& la, core::BStr mask, core::BStr suffix)
{
  char ls[4096];
  //
  dbg_put("scan so: \"%s%s\"\n", mask, suffix);
  snprintf(ls, sizeof(ls), "ls %s%s", mask, suffix);
  FILE *f = popen(ls, "r");
  if(f==NULL)
    return 0;
  //
  size_t gn = 0;
  char line[4096];
  RModuleLibrary::PFD_Data pfd;
  RModuleLibrary::pfd_init(&pfd);
  //
  while(fgets(line, sizeof(line), f)!=NULL) {
    size_t r = strlen(line);
    if(r==0)
      continue;
    if(line[r-1]=='\n') {
      line[--r] = 0;
      if(r==0)
        continue;
    }
    dbg_put("  so: \"%s\"\n", line);
    //
    if(RModuleLibrary::pfd_load(&pfd, line)) {
      dbg_put("  so loaded: \"%s\"\n", line);
      //
      for(int i=0; i<~la; i++) {
        if(reinterpret_cast<RModuleLibrary::PFD_Data*>(la(i)->pfd_data)->module==pfd.module) {
           dbg_put("  so dup with %d\n", i);
           RModuleLibrary::pfd_unload(&pfd);
           goto next;
        }
      }
      //
      dbg_put("  so added\n");
      la.push(new RModuleLibrary(&pfd));
      gn++;
    }
next:;
  }
  dbg_put("/scan so: \"%s%s\"\n", mask, suffix);
  //
  RModuleLibrary::pfd_deinit(&pfd);
  pclose(f);
  //
  return gn;
}

size_t RModuleLibrary::pfd_scan(RModuleLibraryArray& la, const core::DCString& mask)
{
  if(~mask==0) // automatic
    return s_pfd_scan(la, "./*", SO_SUFFIX);
  return s_pfd_scan(la, mask, "") + s_pfd_scan(la, mask, SO_SUFFIX);
}

//***************************************
// ::
//***************************************
