#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// SExecTCL::
//***************************************

void SExecTCL::print_s(const String& val)
{
  M.con().put(cl_Info, "%.*s", int(~val), val.c_str());
}

void SExecTCL::print_s(StringP val)
{
  M.con().put(cl_Info, "%s", val);
}

void SExecTCL::print_f(StringP val, ...)
{
  va_list va;
  va_start(va, val);
  M.con().vput(cl_Info, val, va);
  va_end(va);
}

//***************************************
// ::
//***************************************
