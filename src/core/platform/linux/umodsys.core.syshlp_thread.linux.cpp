#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/linux/syshlp_linux.h>

#include <sys/time.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

void syshlp::t_delay(int msec)
{
  if(msec) {
//    log_put(DL_MAIN_AUX, "{wait %d msec...}\n", msec);
    usleep((long)msec*1000);
//    log_put(DL_MAIN_AUX, "{done %d}\n", msec);
  }
}

unsigned long syshlp::t_msec(void) // overflowed msec timer
{
  struct timeval tv;
//  struct timezone tz;
//  gettimeofday(&tv, &tz);
  gettimeofday(&tv, NULL);
  return (unsigned long)tv.tv_sec*1000 + (unsigned long)tv.tv_usec/1000;
}

//***************************************
// ::
//***************************************

