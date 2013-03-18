#include <umodsys/core/platform/linux/syshlp_linux.h>

#include "../../rsystem/umodsys.base.rsystem.h"
#include "../../rsystem/umodsys.base.rmodule.h"

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
  dbg_put("scan so: \"%s%s\"\n", mask, suffix);
  syshlp::U8String<> ls(mask, suffix), umask(mask, suffix);
  if(!ls)
    return 0;
  char* path = dirname(ls);
//  dbg_put("   scan so path: \"%s\"\n", path);
  DIR *f = opendir(path);
  if(f==NULL)
    return 0;
  //
  size_t gn = 0;
  RModuleLibrary::PFD_Data pfd;
  RModuleLibrary::pfd_init(&pfd);
  //
  struct dirent de, *d;
//  dbg_put("   scan so: \"%s%s\"\n", mask, suffix);
  while(readdir_r(f, &de, &d)==0) {
    if(d==NULL)
      break;
//    dbg_put("   readdir: \"%s\" \n", d->d_name);
    if(d->d_type!=DT_REG && d->d_type!=DT_UNKNOWN)
      continue; // not a file
    if(syshlp::STREQ(d->d_name, ".") || syshlp::STREQ(d->d_name, ".."))
      continue; // special names
//    if(fnmatch("filename", ".so")!=0) {}
    syshlp::U8String<> fullname(path, "/", d->d_name);
    if(!fullname)
      continue; // too long or not matched
//    dbg_put("  match so: \"%s\" like \"%s\"\n", fullname(), umask());
    if(fnmatch(umask(), fullname(), FNM_NOESCAPE)!=0)
      continue; // not matched
    dbg_put("  so: \"%s\"\n", fullname());
    //
    if(RModuleLibrary::pfd_load(&pfd, fullname())) {
      dbg_put("  so loaded: \"%s\"\n", fullname());
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
  closedir(f);
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
