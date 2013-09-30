#include <umodsys/core/platform/linux/syshlp_linux.h>

#include "../../umodsys.base.rsystem.h"
#include "../../umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

#if defined(UMODSYS_ENABLE_DLLEXPORT) || defined(WIN32)
  #define SO_SUFFIX ".dll"
#else
  #define SO_SUFFIX ".so"
#endif

#define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"

typedef IModuleLibraryReg* (*f_get_moduleinfo)(void);

//***************************************
// RModule::
//***************************************

struct RModuleLibrary::PFD_Data {
  void* module;
  f_get_moduleinfo entry;
  IModuleLibraryReg* ilib;
};

//***************************************
//***************************************

bool RModuleLibrary::pfd_init(PFD_Data* pfd)
{
  pfd->module = NULL;
  pfd->entry = NULL;
  pfd->ilib = NULL;
  return true;
}

bool RModuleLibrary::pfd_init(PFD_Data* pfd, PFD_Data* pfdR)
{
  *pfd = *pfdR;
  pfd_init(pfdR);
  return true;
}

IModuleLibraryReg* RModuleLibrary::pfd_load(PFD_Data* pfd, const core::DCString& filename)
{
  if(pfd->module!=NULL)
    return NULL;
//  SetDllDirectory();
  pfd->module = dlopen(filename, RTLD_LOCAL|RTLD_NOW);
//  dbg_put("          dlopen(\"%s\", ...) => %p\n", filename(), pfd->module);
  if(pfd->module==NULL)
    return NULL;
  //
  IModuleLibraryReg* ilib = NULL;
  pfd->entry = (f_get_moduleinfo)(dlsym(pfd->module, MODULE_ENTRY_NAME));
  if(pfd->entry==NULL) {
    dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", filename(), pfd->module, pfd->entry, ilib);
    pfd_unload(pfd);
    return ilib;
  }
  //
  ilib = pfd->entry();
  if(ilib==NULL) {
    dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", filename(), pfd->module, pfd->entry, ilib);
    pfd_unload(pfd);
    return ilib;
  }
  //
  dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", filename(), pfd->module, pfd->entry, ilib);
  return ilib;
}

bool RModuleLibrary::pfd_unload(PFD_Data* pfd)
{
  if(pfd->module!=NULL) {
    dbg_put(rsdl_SoLoad, "          unload so({%p, %p})\n", pfd->module, pfd->entry);
    dlclose(pfd->module);
//    dbg_put("          dlclose(%p)\n", pfd->module);
    pfd->module = NULL;
  }
  pfd->entry = NULL;
  return true;
}

//***************************************
//***************************************

static size_t s_pfd_scan(ISystem* sys, RModuleLibraryArray& la, core::BStr mask, core::BStr suffix)
{
  dbg_put(rsdl_SoLoad, "scan so: \"%s%s\"\n", mask, suffix);
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
    gn += RModuleLibrary::s_add(sys, la, fullname());
next:;
  }
  dbg_put(rsdl_SoLoad, "/scan so: \"%s%s\"\n", mask, suffix);
  //
  closedir(f);
  //
  return gn;
}

size_t RModuleLibrary::pfd_scan(ISystem* sys, RModuleLibraryArray& la, const core::DCString& mask)
{
  if(~mask==0) // automatic
    return s_pfd_scan(sys, la, "./*", SO_SUFFIX);
  return s_pfd_scan(sys, la, mask, "") + s_pfd_scan(sys, la, mask, SO_SUFFIX);
}

//***************************************
// ::
//***************************************
