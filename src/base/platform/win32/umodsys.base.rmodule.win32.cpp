#include <umodsys/core/platform/win32/syshlp_win32.h>

#include "../../rsystem/umodsys.base.rsystem.h"
#include "../../rsystem/umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

#if defined(_M_X64) || defined(_M_IA64)
  #define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"
#else
  #define MODULE_ENTRY_NAME "_UModSys_Plugin_Entry2@8"
#endif
#define SO_SUFFIX ".dll"

typedef IModuleReg* (__stdcall *f_get_moduleinfo)(ISystem* isys, int id);

const int MAX_SO_PATH = 4096;

typedef syshlp::UniPath<MAX_SO_PATH*2> UniSoName;
typedef syshlp::WinPath<MAX_SO_PATH> WinSoName;

//***************************************
// RModuleLibrary::
//***************************************

struct RModuleLibrary::PFD_Data {
  HMODULE module;
  f_get_moduleinfo entry;
};

IModuleReg* RModuleLibrary::pfd_getmr(PFD_Data* pfd, int id)
{
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
  WinSoName filename16(filename);
  if(!filename16)
    return NULL; // path too long
  //
  SetErrorMode(SEM_FAILCRITICALERRORS); 
  dbg_put("  tryload: \"%ls\"\n", filename16());
  pfd->module = LoadLibraryExW(filename16, NULL, 0);
  dbg_put("    LoadLibraryExW(\"%ls\", NULL, 0) => %p\n", filename16(), pfd->module);
  if(pfd->module==NULL)
    return false;
  //
  pfd->entry = (f_get_moduleinfo)(GetProcAddress(pfd->module, MODULE_ENTRY_NAME));
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
    FreeLibrary(pfd->module);
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
  UniSoName mask8(mask, suffix);
//  dbg_put(" -- !mask8=%d of \"%s\" '%s''%s'\n\n", !mask8, mask8(), mask, suffix);
  if(!mask8)
    return 0; // path too long
  WinSoName mask16(mask8);
//  dbg_put(" -- !mask16=%d of \"%ls\"\n\n", !mask16, mask16());
  if(!mask16)
    return 0; // path too long
  WinSoName path16;
  //
  wchar_t *name16;
  int nm = GetFullPathNameW(mask16, path16.Len, path16, &name16);
//  dbg_put(" -- nm=%d, path16.Len=%d\n", nm, path16.Len);
  if(nm>=path16.Len)
    return 0; // path too long
  path16.ok();
  //
  size_t gn = 0;
  WIN32_FIND_DATAW ff;
  HANDLE f = FindFirstFileW(path16, &ff);
//  dbg_put(" -- f=%p of \"%ls\"\n", f, path16());
  if(f==INVALID_HANDLE_VALUE)
    return 0;
  //
  *name16 = 0; // trim to path
  RModuleLibrary::PFD_Data pfd;
  RModuleLibrary::pfd_init(&pfd);
  //
  do {
    dbg_put("  so: \"%ls\"\n", ff.cFileName);
    //
    WinSoName full16(path16, ff.cFileName);
    if(!full16)
      continue; // path too long
    UniSoName full8(full16);
    if(!full8)
      continue; // path too long
    //
//    dbg_put("  tryload: \"%s\"\n", full8());
    if(RModuleLibrary::pfd_load(&pfd, full8())) {
      dbg_put("  so loaded: \"%s\"\n", full8());
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
  } while(FindNextFileW(f, &ff));
  dbg_put("/scan so: \"%s%s\"\n", mask, suffix);
  //
  RModuleLibrary::pfd_deinit(&pfd);
  CloseHandle(f);
  //
  return gn;
}

size_t RModuleLibrary::pfd_scan(RModuleLibraryArray& la, const core::DCString& mask)
{
  if(~mask==0) // automatic
    return s_pfd_scan(la, "*", SO_SUFFIX);
  return s_pfd_scan(la, mask, "") + s_pfd_scan(la, mask, SO_SUFFIX);
}

//***************************************
// ::
//***************************************
