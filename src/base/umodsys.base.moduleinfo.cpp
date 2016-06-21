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

SModuleInfo::SModuleInfo(const char* n, core::Buint16 vh, core::Buint16 vl, const char *i, core::Buint32 bno, core::BTime u)
: name(n), info(i), verno(vh, vl), buildno(bno), updated(u)
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
