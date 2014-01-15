#include <umodsys/base/console.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// SDebug::
//***************************************

SDebug::SDebug(IConsole* ic, uint32 m)
: console(ic), mask(m) {
}

SDebug::~SDebug(void)
{
}

bool SDebug::put(core::byte src, int lev, const char* fmt, ...) const
{
  if((mask & (uint32(1)<<src))==0)
    return false;
  if(!console.valid())
    return false;
  va_list va;
  va_start(va, fmt);
  bool rv = console->vput(lev, fmt, va);
  va_end(va);
  return rv;
}

bool SDebug::dput(core::byte src, const char* fmt, ...) const
{
  if((mask & (uint32(1)<<src))==0)
    return false;
  if(!console.valid())
    return false;
  va_list va;
  va_start(va, fmt);
  bool rv = console->vput(cl_Debug, fmt, va);
  va_end(va);
  return rv;
}

//***************************************
// ::
//***************************************
