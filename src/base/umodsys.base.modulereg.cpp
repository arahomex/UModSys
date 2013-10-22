#include <umodsys/base/module_impl.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// IModuleReg::
//***************************************

IModuleReg::IModuleReg(const SModuleInfo &mi)
: minfo(mi), module(NULL), load_count(0)
{
}

IModuleReg::IModuleReg(const char* n, int vh, int vl, const char *i)
: minfo(n, vh, vl, i), module(NULL), load_count(0)
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
  if(load_count==0) {
    if(!do_open())
      return false;
  }
  load_count++;
  return true;
}

bool IModuleReg::mr_close(void)
{
  load_count--;
  if(load_count==0) 
    return do_close();
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
