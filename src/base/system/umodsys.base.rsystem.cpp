#include "umodsys.base.rsystem.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// RSystem::
//***************************************

void RSystem::ref_add(void) const {}
void RSystem::ref_remove(void) const {}
int  RSystem::ref_links(void) const { return 0; }

//***************************************

SUniquePointerInfo* RSystem::upi_add(const SUniquePointerInfo* lupi)
{
  return NULL;
}

int RSystem::upi_remove(SUniquePointerInfo* upi)
{
  return -1;
}

//***************************************

IConsole* RSystem::get_console(void)
{
  return console;
}

IMemAlloc* RSystem::get_sysmem(void)
{
  return NULL;
}

IMemAlloc* RSystem::get_sharemem(void)
{
  return NULL;
}

//***************************************

IModule* RSystem::module_preload(const DCString& sys_name)
{
  return NULL;
}

IModule* RSystem::module_load(const DCString& name, const SVersion& verno, bool doload)
{
  return NULL;
}

bool RSystem::module_unload(const DCString& name, const SVersion& verno)
{
  return false;
}

bool RSystem::module_unload(IModule* M)
{
  return false;
}

//***************************************
//***************************************

bool RSystem::init(void)
{
  console->put(0, "RSystem::init()\n");
  return false;
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
  return false;
}

//***************************************
//***************************************

RSystem::RSystem(void)
: console(NULL) {
}

RSystem::~RSystem(void)
{
}

//***************************************

RSystem RSystem::s_sys;

//***************************************
// ::
//***************************************
