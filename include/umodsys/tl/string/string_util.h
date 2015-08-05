#ifndef __UMODSYS_TL_STRING_UTIL_H
#define __UMODSYS_TL_STRING_UTIL_H 1
/*************************************************************/
// file: umodsys/tl/string/string_util.h
// info: string utilities
/*************************************************************/

#include <umodsys/tl/string/su_base.h>
#include <umodsys/tl/string/su_char.h>
#include <umodsys/tl/string/su_charw.h>
#include <umodsys/tl/string/su_charl.h>
#include <umodsys/tl/string/su_comparer.h>

#include <umodsys/tl/string/su_coreptr.h>

namespace UModSys {
namespace tl {

//***************************************
// String over comparer
//***************************************

struct SStringComparer {
  template<typename String1, typename String2>
  inline static int compare(const String1& s1, const String2& s2) { 
    return s1.compare(s2); 
  }
};

//***************************************
// 
//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_STRING_UTIL_H
