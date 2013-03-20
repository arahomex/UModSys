#include <umodsys/base/module_impl.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// IModuleReg::
//***************************************

IModuleReg::IModuleReg(void)
: module(NULL), load_count(0)
{
}

IModuleReg::~IModuleReg(void)
{
}

bool IModuleReg::mr_isopen(void) const
{
  return load_count>0;
}


bool IModuleReg::mr_open(void)
{
  load_count++;
  return true;
}

bool IModuleReg::mr_close(void)
{
  load_count--;
  return true;
}

//***************************************
// IModuleLibraryReg::
//***************************************

IModuleLibraryReg::IModuleLibraryReg(void)
{
}

IModuleLibraryReg::~IModuleLibraryReg(void)
{
}

//***************************************
// ::
//***************************************
