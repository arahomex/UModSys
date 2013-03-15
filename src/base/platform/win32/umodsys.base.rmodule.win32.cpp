#include "../../rsystem/umodsys.base.rsystem.h"
#include "../../rsystem/umodsys.base.rmodule.h"

#include <windows.h>

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

#define MAX_SO_PATH 4096

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
  wchar_t filename16[MAX_SO_PATH+1];
  int nm = MultiByteToWideChar(CP_UTF8, 0, filename, ~filename, filename16, MAX_SO_PATH);
  if(nm==0)
    return false; // path too long
  filename16[nm] = 0;
  //
  SetErrorMode(SEM_FAILCRITICALERRORS); 
  pfd->module = LoadLibraryExW(filename16, NULL, 0);
  dbg_put("    LoadLibraryExW(\"%ls\", NULL, 0) => %p\n", filename16, pfd->module);
  if(pfd->module==NULL)
    return false;
  //
  pfd->entry = (f_get_moduleinfo)(GetProcAddress(pfd->module, MODULE_ENTRY_NAME));
  if(pfd->entry==NULL) {
    pfd_unload(pfd);
    return false;
  }
  //
/*
  pfd->ireg = pfd->entry(&RSystem::s_sys, sys_id);
  if(pfd->ireg==NULL) {
    pfd_unload(pfd);
    return false;
  }
*/
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
  char msk8[MAX_SO_PATH+1];
  wchar_t msk16[MAX_SO_PATH+1];
  wchar_t path16[MAX_SO_PATH+1], *name16;
  wchar_t sopath16[MAX_SO_PATH+1];
  int nm;
  //
  dbg_put("scan so: \"%s%s\"\n", mask, suffix);
  if(_snprintf_s(msk8, MAX_SO_PATH, MAX_SO_PATH, "%s%s", mask, suffix)<=0)
    return NULL; // path too long
  nm = MultiByteToWideChar(CP_UTF8, 0, msk8, strlen(msk8), msk16, MAX_SO_PATH);
  if(nm==0)
    return NULL; // path too long
  msk16[nm] = 0;
  nm = GetFullPathNameW(msk16, MAX_SO_PATH, path16, &name16);
  if(nm>=MAX_SO_PATH)
    return NULL; // path too long
  //
  size_t gn = 0;
  WIN32_FIND_DATAW ff;
  HANDLE f = FindFirstFileW(path16, &ff);
  if(f==INVALID_HANDLE_VALUE)
    return 0;
  //
  *name16 = 0; // trim to path
  char fname[4096];
  RModuleLibrary::PFD_Data pfd;
  RModuleLibrary::pfd_init(&pfd);
  //
  do {
    dbg_put("  so: \"%ls\"\n", ff.cFileName);
    //
    if(_snwprintf_s(msk16, MAX_SO_PATH, MAX_SO_PATH, L"%s%s", path16, ff.cFileName)<=0)
      continue; // path too long
    //
    char msk8[MAX_SO_PATH];
    nm = WideCharToMultiByte(CP_UTF8, 0, msk16, wcslen(msk16), msk8, MAX_SO_PATH, NULL, NULL);
    if(nm==0)
      continue; // path too long
    msk8[nm] = 0;
    //
    if(RModuleLibrary::pfd_load(&pfd, msk8)) {
      dbg_put("  so loaded: \"%s\"\n", msk8);
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
