#ifndef __UMODSYS_TYPES_STRING_UTILS_CHARL_H
#define __UMODSYS_TYPES_STRING_UTILS_CHARL_H 1
/*************************************************************/
// file: umodsys/tl/string/su_charl.h
// info: string utilities [utf32]
/*************************************************************/

#include <umodsys/tl/string/su_base.h>

namespace UModSys {
namespace tl {
namespace su {

template<>
struct TCharOptions<core::BCharL> {
  typedef core::BCharL       Char;
  typedef core::BCharLIndex  Index;
  typedef core::BCStrL       CStr;
  typedef core::BOStrL       OStr;
  //
};

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_CHARL_H
