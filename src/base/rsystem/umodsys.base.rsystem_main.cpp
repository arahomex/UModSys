#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RSystem::
//***************************************

const DCString& RSystem::uptr_string(const DCString& v)
{
  return *uptr_pool.append(v);
}

const DCString& RSystem::mod_string(const DCString& v)
{
  return *mod_pool.append(v);
}

//***************************************

bool RSystem::init(void)
{
#if defined(_DEBUG) && defined(_MSC_VER)
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_CHECK_CRT_DF|_CRTDBG_ALLOC_MEM_DF);
//  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_CHECK_CRT_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF);
//  _CrtSetDbgFlag(/*_CRTDBG_DELAY_FREE_MEM_DF|*/_CRTDBG_ALLOC_MEM_DF);
#endif
  //
  dbg_put("RSystem::init()\n");
  //
  SUniquePointer::s_resolve(this); // initalize all upis
  { // list all upis
    dbg_put("RSystem::init() -- dump upi {S:%d C:%d}\n", uptr_pool.used_strings(), uptr_pool.used_chars());
    for(const SUniquePointer* x=SUniquePointer::root.next; x!=&SUniquePointer::root; x=x->next) {
      if(x==NULL) {
        dbg_put("upi NULL\n");
        break;
      }
      dbg_put("upi{%s %s(%d) %p}\n", x->info.group, x->info.name, x->info.verno, x->upi);
    }
    dbg_put("RSystem::init() -- /dump upi\n");
  }
  //
//  sys_library = new RModuleLibrary();
  return true;
}

bool RSystem::exec_args(int argc, char** argv)
{
  dbg_put("RSystem::exec_args(%d)\n", argc);
  return false;
}

bool RSystem::exec_main(void)
{
  dbg_put("RSystem::exec_main()\n");
  return false;
}

bool RSystem::deinit(void)
{
  dbg_put("RSystem::deinit()\n");
  //
  SUniquePointer::s_unresolve(this); // deinitalize all upis
  //
  return true;
}

//***************************************
//***************************************

RSystem::RSystem(void)
: console(NULL)/*, sys_library(NULL)*/ {
}

RSystem::~RSystem(void)
{
}

//***************************************

RSystem RSystem::s_sys;

//***************************************
// base::
//***************************************

void base::dbg_put(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  RSystem::s_sys.get_console()->vput(0, fmt, va);
  va_end(va);
}

//***************************************
// ::
//***************************************
