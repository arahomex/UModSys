#include <umodsys/common/stdtypedefs.h>
#include <umodsys/common/arch.h>

using namespace UModSys;
using namespace UModSys::core;

//***************************************
// core::
//***************************************

BTime core::null_time = 0;

unsigned char core::lost(const void* data)
{
  return 0;
}

//***************************************
// ::
//***************************************
