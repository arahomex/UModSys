#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/win32/syshlp_win32.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

void syshlp::delay(int msec)
{
  if(msec) {
//    log_put(DL_MAIN_AUX, "{wait %d msec...}\n", msec);
    Sleep(msec);
//    log_put(DL_MAIN_AUX, "{done %d}\n", msec);
  }
}

//***************************************
// ::
//***************************************

