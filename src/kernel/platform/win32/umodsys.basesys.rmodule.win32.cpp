#include <umodsys/common/platform/win32/syshlp_win32.h>

#include "../../umodsys.basesys.rsystem.h"
#include "../../umodsys.basesys.rmodule.h"

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

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

typedef IModuleLibraryReg* (__stdcall *f_get_moduleinfo)(void);

const int MAX_SO_PATH = 4096;

typedef syshlp::UniPath<MAX_SO_PATH*2> UniSoName;
typedef syshlp::WinPath<MAX_SO_PATH> WinSoName;

static UniSoName s_pwd;

static void s_pwd_init(void)
{
  if(**s_pwd==0) {
    wchar_t pwd[MAX_SO_PATH], pwd2[MAX_SO_PATH], *fn;
    if(GetModuleFileNameW(NULL, pwd, MAX_SO_PATH)!=0 && GetFullPathNameW(pwd, MAX_SO_PATH, pwd2, &fn)!=0) {
      *fn = 0;
      s_pwd.set(pwd2);
    }
  }
}

//***************************************
// RModuleLibrarySO::
//***************************************

struct RModuleLibrarySO::PFD_Data {
  HMODULE module;
  f_get_moduleinfo entry;
};

//***************************************
//***************************************

bool RModuleLibrarySO::pfd_init(PFD_Data* pfd)
{
  pfd->module = NULL;
  pfd->entry = NULL;
  return true;
}

bool RModuleLibrarySO::pfd_init(PFD_Data* pfd, PFD_Data* pfdR)
{
  *pfd = *pfdR;
  pfd_init(pfdR);
  return true;
}

//***************************************
//***************************************

IModuleLibraryReg* RModuleLibrarySO::pfd_load(PFD_Data* pfd, const core::DCString& filename)
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

bool RModuleLibrarySO::pfd_unload(PFD_Data* pfd)
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

static size_t s_pfd_scan(ISystem* sys, RModuleLibrarySOArray& la, core::BStr path, core::BStr mask, core::BStr suffix)
{
  s_pwd_init();
  dbg_put(rsdl_SoLoad, "scan so: \"%s%s%s\"\n", path, mask, suffix);
  UniSoName mask8(path, mask, suffix);
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
    if(ff.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_OFFLINE))
      continue;
//    dbg_put(rsdl_SoLoad, "  so: \"%ls\"\n", ff.cFileName);
    //
    WinSoName full16(path16, ff.cFileName);
    if(!full16)
      continue; // path too long
    SetErrorMode(SEM_FAILCRITICALERRORS); 
    HMODULE m = LoadLibraryExW(full16, NULL, DONT_RESOLVE_DLL_REFERENCES|LOAD_LIBRARY_AS_DATAFILE);
    if(m==NULL)
      continue;
    FreeLibrary(m);
    if(m==GetModuleHandleW(NULL))
      continue; // this executable
    UniSoName full8(full16);
    if(!full8)
      continue; // path too long
    gn += RModuleLibrarySO::s_add(sys, la, full8());
//next:;
  } while(FindNextFileW(f, &ff));
  dbg_put(rsdl_SoLoad, "/scan so: \"%s%s\"\n", mask, suffix);
  FindClose(f);
  //
  return gn;
}

inline static size_t s_pfd_scan(ISystem* sys, RModuleLibrarySOArray& la, core::BStr mask, core::BStr suffix)
{
  s_pwd_init();
  return s_pfd_scan(sys, la, "./", mask, suffix) + s_pfd_scan(sys, la, s_pwd, mask, suffix);
}

size_t RModuleLibrarySO::pfd_scan(ISystem* sys, RModuleLibrarySOArray& la, const core::DCString& mask)
{
  if(~mask==0) // automatic
    return s_pfd_scan(sys, la, "*", SO_SUFFIX);
  return s_pfd_scan(sys, la, mask, "") + s_pfd_scan(sys, la, mask, SO_SUFFIX);
}

//***************************************
// RSystem::
//***************************************

bool RSystem::platform_init(void)
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
  //
#if defined(_DEBUG) && defined(_MSC_VER)
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_ALLOC_MEM_DF);
//  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_CHECK_CRT_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF);
#endif
  //
  return true;
}

#if defined(_DEBUG) && defined(_MSC_VER)
static void s_dumper(void *pHeap, void *pSystem)
{
  long rq;
  char *fn;
  int line;
  size_t len = _msize_dbg(pHeap, _CLIENT_BLOCK);
  int rv = _CrtIsMemoryBlock(pHeap, len, &rq, &fn, &line);
  if(rv) {
    dbg_put(rsdl_System, "  leak: %p:%ld @%ld %s(%d)\n", pHeap, (long)len, rq, fn, line);
  }
}
#endif

bool RSystem::platform_deinit(void)
{
#if defined(_DEBUG) && defined(_MSC_VER)
//  UMODSYS_MALLOC(10, __FILE__, __LINE__);
  _CrtDoForAllClientObjects(s_dumper, this);
//  _CrtDumpMemoryLeaks();
  _CrtSetDbgFlag(0);
#endif
  return true;
}

//***************************************
// ::
//***************************************
