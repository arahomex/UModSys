#include <umodsys/common/platform/linux/syshlp_linux.h>

#include "../../umodsys.basesys.rsystem.h"
#include "../../umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

#if defined(UMODSYS_ENABLE_DLLEXPORT) || defined(WIN32)
  #define SO_SUFFIX ".dll\0\0"
#else
  #define SO_SUFFIX ".so\0\0"
#endif

#define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"

typedef IModuleLibraryReg* (*f_get_moduleinfo)(void);

const int MAX_SO_PATH = 4096;

static char s_pwd[MAX_SO_PATH];

static void s_pwd_init(void)
{
  if(*s_pwd==0) {
    if (readlink ("/proc/self/exe", s_pwd, MAX_SO_PATH) != -1) {
      dirname(s_pwd);
      strcat(s_pwd, "/");
    } else {
      strcpy(s_pwd, ".");
    }
  }
}

//***************************************
// RModule::
//***************************************

struct RModuleLibrarySO::PFD_Data {
  void* module;
  f_get_moduleinfo entry;
  IModuleLibraryReg* ilib;
};

//***************************************
//***************************************

bool RModuleLibrarySO::pfd_init(PFD_Data* pfd)
{
  pfd->module = NULL;
  pfd->entry = NULL;
  pfd->ilib = NULL;
  return true;
}

bool RModuleLibrarySO::pfd_init(PFD_Data* pfd, PFD_Data* pfdR)
{
  *pfd = *pfdR;
  pfd_init(pfdR);
  return true;
}

IModuleLibraryReg* RModuleLibrarySO::pfd_load(PFD_Data* pfd, const core::DCString& filename)
{
  if(pfd->module!=NULL)
    return NULL;
//  SetDllDirectory();
  pfd->module = dlopen(*filename, RTLD_LOCAL|RTLD_DEEPBIND|RTLD_NOW);
//  dbg_put(rsdl_SoLoad, "          dlopen(\"%s\", ...) => %p\n", *filename, pfd->module);
  if(pfd->module==NULL) {
    dbg_put(rsdl_SoLoad, "          dlopen(\"%s\", ...) error: %s\n", *filename, dlerror());
    return NULL;
  }
  //
  IModuleLibraryReg* ilib = NULL;
  pfd->entry = (f_get_moduleinfo)(dlsym(pfd->module, MODULE_ENTRY_NAME));
  if(pfd->entry==NULL) {
    dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", *filename, pfd->module, pfd->entry, ilib);
    pfd_unload(pfd);
    return ilib;
  }
  //
  ilib = pfd->entry();
  if(ilib==NULL) {
    dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", *filename, pfd->module, pfd->entry, ilib);
    pfd_unload(pfd);
    return ilib;
  }
  //
  dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", *filename, pfd->module, pfd->entry, ilib);
  return ilib;
}

bool RModuleLibrarySO::pfd_unload(PFD_Data* pfd)
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

static size_t s_pfd_scan(IKernel* sys, RModuleLibrarySOArray& la, core::BStr bpath, core::BStr mask, core::BStr suffix)
{
  dbg_put(rsdl_SoLoad, "scan so: \"%s%s%s\"\n", bpath, mask, suffix);
  syshlp::U8String<> ls(bpath, mask, suffix), umask(bpath, mask, suffix);
  if(!ls)
    return 0;
  char* path = dirname(ls);
//  dbg_put(rsdl_SoLoad, "   scan so path: \"%s\"\n", path);
  DIR *f = opendir(path);
  if(f==NULL)
    return 0;
  //
  size_t gn = 0;
  //
  struct dirent de, *d;
//  dbg_put(rsdl_SoLoad, "   scan so: \"%s\"\n", mask, suffix);
  while(readdir_r(f, &de, &d)==0) {
    if(d==NULL)
      break;
//    dbg_put(rsdl_SoLoad, "   readdir: \"%s\" \n", d->d_name);
    if(d->d_type!=DT_REG && d->d_type!=DT_UNKNOWN)
      continue; // not a file
    if(syshlp::STREQ(d->d_name, ".") || syshlp::STREQ(d->d_name, ".."))
      continue; // special names
//    if(fnmatch("filename", ".so")!=0) {}
    syshlp::U8String<> fullname(path, "/", d->d_name);
    if(!fullname)
      continue; // too long or not matched
//    dbg_put(rsdl_SoLoad, "  match so: \"%s\" like \"%s\"\n", fullname(), umask());
    if(fnmatch(umask(), fullname(), FNM_NOESCAPE)!=0)
      continue; // not matched
    dbg_put(rsdl_SoLoad, "  matched so: \"%s\" like \"%s\"\n", fullname(), umask());
    gn += RModuleLibrarySO::s_add(sys, la, fullname());
next:;
  }
  dbg_put(rsdl_SoLoad, "/scan so: \"%s%s%s\"\n", bpath, mask, suffix);
  //
  closedir(f);
  //
  return gn;
}

inline static size_t s_pfd_scan(IKernel* sys, RModuleLibrarySOArray& la, core::BStr mask, core::BStr suffix)
{
  return s_pfd_scan(sys, la, "./", mask, suffix) + s_pfd_scan(sys, la, s_pwd, mask, suffix);
}

size_t RModuleLibrarySO::pfd_scan(IKernel* sys, RModuleLibrarySOArray& la, const core::DCString& mask)
{
  s_pwd_init();
  if(~mask==0) // automatic
    return s_pfd_scan(sys, la, "*", SO_SUFFIX);
  return s_pfd_scan(sys, la, *mask, "") + s_pfd_scan(sys, la, *mask, SO_SUFFIX);
}

//***************************************
// RSystemKernel::
//***************************************

bool RSystemKernel::platform_init(void)
{
  rsys_dbg.mdisable();
  rsys_dbg.enable(rsdl_SysTests);
  rsys_dbg.enable(rsdl_MemoryError);
//  rsys_dbg.enable(rsdl_Uid);
//  rsys_dbg.enable(rsdl_System);
  rsys_dbg.enable(rsdl_Module);
  rsys_dbg.enable(rsdl_ModuleLibrary);
  rsys_dbg.enable(rsdl_SoLoad);
  rsys_dbg.enable(rsdl_TCL);
  return true;
}

bool RSystemKernel::platform_deinit(void)
{
  return true;
}

//***************************************
// ::
//***************************************
