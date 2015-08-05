#ifndef __UMODSYS_TL_METASTL_UTILS_H
#define __UMODSYS_TL_METASTL_UTILS_H 1
/*************************************************************/
// file: umodsys/tl/metastl/utils.h
// info: utilities: pair
/*************************************************************/

#include <umodsys/tl/composite/pair.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename A, typename B>
struct pair : public TPair<A, B> {
  inline pair(const A& a, const B& b) : TPair<A, B>(a, b) {}
  inline pair(void) {}
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_METASTL_UTILS_H
