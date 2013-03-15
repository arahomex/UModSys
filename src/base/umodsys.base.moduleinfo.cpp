#include <umodsys/base/module.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// SModuleInfo::
//***************************************

SModuleInfo::~SModuleInfo(void) 
{
}

SModuleInfo::SModuleInfo(const char* n, int vh, int vl, const char *i)
: name(n), info(i), verno(vh, vl) 
{
}

SModuleInfo::SModuleInfo(void) 
{
}

void SModuleInfo::set(const char* n, int vh, int vl, const char *i)
{
  name = n;
  verno = SVersion(vh, vl);
  info = i;
}

//***************************************
// ::
//***************************************
