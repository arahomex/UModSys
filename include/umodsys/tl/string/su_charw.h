#ifndef __UMODSYS_TYPES_STRING_UTILS_CHARW_H
#define __UMODSYS_TYPES_STRING_UTILS_CHARW_H 1
/*************************************************************/
// file: umodsys/tl/string/su_charw.h
// info: string utilities [utf16]
/*************************************************************/

#include <umodsys/tl/string/su_base.h>

namespace UModSys {
namespace tl {
namespace su {

template<>
struct TCharOptions<core::BCharW> {
  typedef core::BCharW       Char;
  typedef core::BCharWIndex  Index;
  typedef core::BCStrW       CStr;
  typedef core::BOStrW       OStr;
  //
};

template<> 
inline size_t slen(const core::BCharW* s) 
{ 
  return s==NULL || *s==0 ? 0 : wcslen(s); 
}

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_CHARW_H
