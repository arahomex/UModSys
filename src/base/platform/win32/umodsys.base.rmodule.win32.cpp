#include <umodsys/core/platform/win32/syshlp_win32.h>

#include "../../rsystem/umodsys.base.rsystem.h"
#include "../../rsystem/umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

#if defined(_M_X64) || defined(_M_IA64)
  #define MODULE_ENTRY_NAME "UModSys_Plugin_Entry2"
#else
  #define MODULE_ENTRY_NAME "_UModSys_Plugin_Entry2@0"
#endif
#define SO_SUFFIX ".dll"

typedef IModuleLibraryReg* (__stdcall *f_get_moduleinfo)(void);

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

//***************************************
//***************************************

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

//***************************************
//***************************************

IModuleLibraryReg* RModuleLibrary::pfd_load(PFD_Data* pfd, const core::DCString& filename)
{
  if(pfd->module!=NULL)
    return NULL;
//  SetDllDirectory();
  WinSoName filename16(filename);
  if(!filename16)
    return NULL; // path too long
  //
  SetErrorMode(SEM_FAILCRITICALERRORS); 
//  dbg_put("       tryload: \"%ls\"\n", filename16());
  pfd->module = LoadLibraryExW(filename16, NULL, 0);
  if(pfd->module==NULL)
    return NULL;
  //
//  dbg_put("          LoadLibraryExW(\"%ls\", NULL, 0) => %p\n", filename16(), pfd->module);
  IModuleLibraryReg* ilib = NULL;
  pfd->entry = (f_get_moduleinfo)(GetProcAddress(pfd->module, MODULE_ENTRY_NAME));
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
  dbg_put(rsdl_SoLoad, "          load so(\"%s\") {%p, %p} => %p\n", filename16(), pfd->module, pfd->entry, ilib);
  return ilib;
}

bool RModuleLibrary::pfd_unload(PFD_Data* pfd)
{
  if(pfd->module!=NULL) {
    dbg_put(rsdl_SoLoad, "          unload so({%p, %p})\n", pfd->module, pfd->entry);
    BOOL rv = FreeLibrary(pfd->module);
//    dbg_put("          FreeLibrary(%p) => %d\n", pfd->module, (int)rv);
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
  do {
    dbg_put(rsdl_SoLoad, "  so: \"%ls\"\n", ff.cFileName);
    //
    WinSoName full16(path16, ff.cFileName);
    if(!full16)
      continue; // path too long
    UniSoName full8(full16);
    if(!full8)
      continue; // path too long
    gn += RModuleLibrary::s_add(sys, la, full8());
next:;
  } while(FindNextFileW(f, &ff));
  dbg_put(rsdl_SoLoad, "/scan so: \"%s%s\"\n", mask, suffix);
  FindClose(f);
  //
  return gn;
}

size_t RModuleLibrary::pfd_scan(ISystem* sys, RModuleLibraryArray& la, const core::DCString& mask)
{
  if(~mask==0) // automatic
    return s_pfd_scan(sys, la, "*", SO_SUFFIX);
  return s_pfd_scan(sys, la, mask, "") + s_pfd_scan(sys, la, mask, SO_SUFFIX);
}

//***************************************
// ::
//***************************************
