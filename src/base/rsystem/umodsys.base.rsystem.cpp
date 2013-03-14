#include "umodsys.base.rsystem.h"
#include "umodsys.base.rmodule.h"

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

size_t RSystem::module_preload(const core::DCString& sys_name)
{
  return 0;
}

size_t RSystem::module_findall(IModule* amodules[], size_t nmodules, const core::DCString& mask)
{
  return 0;
}

IModule* RSystem::module_find(const core::DCString& name, const core::SVersion& verno)
{
  return NULL;
}

//***************************************
//***************************************

bool RSystem::init(void)
{
  console->put(0, "RSystem::init()\n");
  sys_library = new RModuleLibrary();
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
