#ifndef __UMODSYS_TL_METASTL_THROWS_H
#define __UMODSYS_TL_METASTL_THROWS_H 1
/*************************************************************/
// file: umodsys/tl/metastl/throws.h
// info: throws exception
/*************************************************************/

#include <umodsys/core/stdcore.h>
#include <umodsys/core/common_exceptions.h>

namespace UModSys {
namespace tl {

/*************************************************************/

void throw_memoryerror(void) UMODSYS_THROW((core::EError));

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_METASTL_THROWS_H
