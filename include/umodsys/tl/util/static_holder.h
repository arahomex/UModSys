#ifndef __UMODSYS_TL_UTIL_STATICHOLDER_H
#define __UMODSYS_TL_UTIL_STATICHOLDER_H 1
/*************************************************************/
// file: umodsys/tl/util/static_holder.h
// info: fixed array of elements
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename T>
struct TTypeStaticHolder {
  typedef T Type;
  //
  T* pointer;
  char binary[sizeof(T)];
  bool finit;
  //
  inline TTypeStaticHolder(void) : finit(false) { pointer=reinterpret_cast<T*>(binary); }
  inline ~TTypeStaticHolder(void) { clear(); }
  //
  inline T* operator->(void) { return finit ? pointer : NULL; }
  inline const T* operator->(void) const { return finit ? pointer : NULL; }
  inline operator T*(void) { return finit ? pointer : NULL; }
  inline operator const T*(void) const { return finit ? pointer : NULL; }
  //
  inline bool valid(void) const { return finit; }
  inline bool invalid(void) const { return !finit; }
  //
  inline void _construct(void) { ::new(pointer) T(); }
  inline void _destruct(void) { pointer->~T(); }
  //
  inline void clear(void) { deinit(); }
  inline T* init(void) { if(!finit) { _construct(); finit=true; } return pointer; }
  inline void deinit(void) { if(finit) { _destruct(); finit=false; } }
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_UTIL_STATICHOLDER_H
