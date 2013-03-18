#ifndef __UMODSYS_CORE_PLATFORM_LINUX_SYSHLP_H
#define __UMODSYS_CORE_PLATFORM_LINUX_SYSHLP_H 1
/*************************************************************/
// file: umodsys/core/platform/linux/syshlp.h
// info: sys helper for linuc platform
/*************************************************************/

#include "../../stddefs.h"

/*******************************************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define DEFAULT_DIR_MODE 0777
#define DEFAULT_FILE_MODE 0666

/*******************************************************************************/

namespace UModSys {
namespace core {
namespace syshlp {

/////////////////////////////////////////////////////////////////////////////
// DEFS

/////////////////////////////////////////////////////////////////////////////
// OS PATH INLINES

/////////////////////////////////////////////////////////////////////////////
// INLINES

inline FILE* c_fopen(const char *cfilename, const char *cmode)
{
  return fopen(cfilename, cmode);
}


/////////////////////////////////////////////////////////////////////////////
// END

} // namespace syshlp
} // namespace core
} // namespace UModSys

#endif

