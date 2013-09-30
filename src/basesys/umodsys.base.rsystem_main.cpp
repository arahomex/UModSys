#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

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

const DCString& RSystem::uptr_string(const DCString& v)
{
  return *uptr_pool.append(v);
}

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
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_CHECK_CRT_DF|_CRTDBG_ALLOC_MEM_DF);
//  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_CHECK_CRT_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF);
#endif
  //
  dbg_put(rsdl_System, "RSystem::init()\n");
  //
  SUniquePointer::s_resolve(this); // initalize all upis
  { // list all upis
    dbg_put(rsdl_System, "RSystem::init() -- dump upi {S:%d C:%d}\n", uptr_pool.used_strings(), uptr_pool.used_chars());
    for(const SUniquePointer* x=SUniquePointer::root.next; x!=&SUniquePointer::root; x=x->next) {
      if(x==NULL) {
        dbg_put(rsdl_System, "upi NULL\n");
        break;
      }
      dbg_put(rsdl_System, "upi{%s %s(%d) %p}\n", x->info.group, x->info.name, x->info.verno, x->upi);
    }
    dbg_put(rsdl_System, "RSystem::init() -- /dump upi\n");
  }
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

bool RSystem::deinit(void)
{
  dbg_put(rsdl_System, "RSystem::deinit()\n");
  //
  moddb.cleanup();
  SUniquePointer::s_unresolve(this); // deinitalize all upis
  //
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

RSystem::RSystem(void)
: console(NULL), moddb(this) /*, sys_library(NULL)*/ 
{
  params = new RParameters(this);
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