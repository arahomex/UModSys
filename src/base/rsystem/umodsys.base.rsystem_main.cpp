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
  return true;
}

//***************************************
//***************************************

RSystem::RSystem(void)
: console(NULL), sys_library(NULL) {
}

RSystem::~RSystem(void)
{
}

//***************************************

RSystem RSystem::s_sys;

//***************************************
// ::
//***************************************
