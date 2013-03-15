#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

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
  console->put(0, "RSystem::init()\n");
  //
  SUniquePointer::s_resolve(this); // initalize all upis
  { // list all upis
    console->put(0, "RSystem::init() -- dump upi {S:%d C:%d}\n", uptr_pool.used_strings(), uptr_pool.used_chars());
    for(const SUniquePointer* x=SUniquePointer::root.next; x!=&SUniquePointer::root; x=x->next) {
      if(x==NULL) {
        console->put(0, "upi NULL\n");
        break;
      }
      console->put(0, "upi{%s %s(%d) %p}\n", x->info.group, x->info.name, x->info.verno, x->upi);
    }
    console->put(0, "RSystem::init() -- /dump upi\n");
  }
  //
//  sys_library = new RModuleLibrary();
  return true;
}

bool RSystem::exec_args(int argc, char** argv)
{
  console->put(0, "RSystem::exec_args(%d)\n", argc);
  return false;
}

bool RSystem::exec_main(void)
{
  console->put(0, "RSystem::exec_main()\n");
  return false;
}

bool RSystem::deinit(void)
{
  console->put(0, "RSystem::deinit()\n");
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
// ::
//***************************************
