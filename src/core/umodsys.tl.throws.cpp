#include "umodsys/tl/metastl/throws.h"

using namespace UModSys;
using namespace tl;

//***************************************
// namespace tl::
//***************************************

void tl::throw_memoryerror(const UModSys::core::SSourceContext* ctx)
UMODSYS_THROW((core::EError)) 
{
  throw core::EError(ctx, 0, 0);
}
