#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystem::
//***************************************

bool RSystem::init(void)
{
  rsys_dbg.mask = 0;
  rsys_dbg.enable(rsdl_System);
  rsys_dbg.enable(rsdl_Module);
  rsys_dbg.enable(rsdl_ModuleLibrary);
  rsys_dbg.enable(rsdl_SoLoad);
  //
#if defined(_DEBUG) && defined(_MSC_VER)
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_ALLOC_MEM_DF);
//  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_CHECK_CRT_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF);
#endif
  //
  dbg_put(rsdl_System, "RSystem::init()\n");
  //
  SUniquePointer::s_resolve(this); // initalize all upis
//  dump_upis(); // list all upis
  params = new(M()) RParameters(this);
  moddb.mod_this = new(M()) RModuleLibraryThis(this);
  moddb.mod_this->load0();
  moddb.mod_this->scan();
  moddb.mod_this->cleanup();
  //
//  sys_library = new RModuleLibrary();
  return true;
}

bool RSystem::exec_args(int argc, char** argv)
{
  dbg_put(rsdl_System, "RSystem::exec_args(%d)\n", argc);
  return false;
}

bool RSystem::exec_main(void)
{
  dbg_put(rsdl_System, "RSystem::exec_main()\n");
  return false;
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

bool RSystem::deinit(void)
{
  dbg_put(rsdl_System, "RSystem::deinit()\n");
  //
  moddb.cleanup();
  SUniquePointer::s_unresolve(this); // deinitalize all upis
  //
  params.clear();
  //
  uptr_strings.free();
  uptr_list.free();
  //
  sc_strings.free();
  sc_list.free();
  //
#if defined(_DEBUG) && defined(_MSC_VER)
//  UMODSYS_MALLOC(10, __FILE__, __LINE__);
  _CrtDoForAllClientObjects(s_dumper, this);
//  _CrtDumpMemoryLeaks();
  _CrtSetDbgFlag(0);
#endif
  return true;
}

void RSystem::set_console(IConsole* cc)
{
  console = cc;
  rsys_dbg.console = cc;
}

//***************************************

size_t RSystem::find_shells(IRefObject::TypeId tids[], size_t ntids)
{
  return moddb.moduledb_findobjname(IShell::_get_interface_type(), tids, ntids);
}

//***************************************
//***************************************

void RSystem::dump_upis(void)
{
  dbg_put(rsdl_System, "RSystem::dump_upis() {S:%d C:%d} %u\n", uptr_strings.used_strings(), uptr_strings.used_chars(), (unsigned)~uptr_list);
/*
  for(const SUniquePointer* x=SUniquePointer::root.next; x!=&SUniquePointer::root; x=x->next) {
    if(x==NULL) {
      dbg_put(rsdl_System, "upi NULL\n");
      break;
    }
    dbg_put(rsdl_System, "upi{%s %s(%d) %p}\n", x->info.group, x->info.name, x->info.verno, x->upi);
  }
*/
  for(size_t i=0, n=~uptr_list; i<n; i++) {
    const SUniquePointerInfo* x = uptr_list(i);
    if(x==NULL) {
      dbg_put(rsdl_System, "  %u upi NULL\n", (unsigned)i);
      continue;
    }
    dbg_put(rsdl_System, "  %u %p upi{%s %s(%d)}\n", (unsigned)i, x, x->group, x->name, x->verno);
  }
  dbg_put(rsdl_System, "RSystem::dump_upis() / \n");
}

//***************************************
//***************************************

RSystem::RSystem(void)
: console(NULL), moddb(this), /* sys_library(NULL),*/ 
  mema_shared(NULL), mema_system(NULL)
{
}

RSystem::~RSystem(void)
{
}

//***************************************

RSystem RSystem::s_sys;

//***************************************
// rsystem::
//***************************************

SDebug rsystem::rsys_dbg;

void rsystem::dbg_put(eRSystemDebugLevels dl, const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  rsys_dbg.dvput(dl, fmt, va);
  va_end(va);
}

void rsystem::err_put(eRSystemDebugLevels dl, const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  rsys_dbg.vput(dl, cl_Error, fmt, va);
  va_end(va);
}

//***************************************
// ::
//***************************************
