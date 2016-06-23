#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystem::
//***************************************

bool RSystem::init(void)
{
  if(!platform_init())
    return false;
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
  return true;
}

bool RSystem::exec_main(void)
{
  dbg_put(rsdl_System, "RSystem::exec_main()\n");
//  exec_tests();
  return false;
}

bool RSystem::deinit(void)
{
  dbg_put(rsdl_System, "RSystem::deinit()\n");
  //
  shells.clear();
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
  return platform_deinit();
}

void RSystem::set_console(IConsole* cc)
{
  console = cc;
  rsys_dbg.console = cc;
}

//***************************************

size_t RSystem::find_shells(IRefObject::TypeId tids[], size_t ntids, BCStr mask)
{
  return moddb.moduledb_findobjname(IShell::_root_get_interface_type(), tids, ntids, mask);
}

//***************************************
//***************************************

void RSystem::dump_upis(void)
{
  dbg_put(rsdl_Uid, "RSystem::dump_upis() {S:%d C:%d} %u\n", uptr_strings.used_strings(), uptr_strings.used_chars(), (unsigned)~uptr_list);
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
      dbg_put(rsdl_Uid, "  %u upi NULL\n", (unsigned)i);
      continue;
    }
    dbg_put(rsdl_Uid, "  %u %p upi{%s %s(%d)}\n", (unsigned)i, x, x->group, x->name, x->verno);
  }
  dbg_put(rsdl_Uid, "RSystem::dump_upis() / \n");
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
