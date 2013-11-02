#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/linux/syshlp_linux.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************
#if 0

void syshlp::delay(int msec)
{
  if(msec) {
//    log_put(DL_MAIN_AUX, "{wait %d msec...}\n", msec);
    Sleep(msec);
//    log_put(DL_MAIN_AUX, "{done %d}\n", msec);
  }
}

#endif

//***************************************
// ::
//***************************************

